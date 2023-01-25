/*
  +----------------------------------------------------------------------+
  | Copyright (c) The PHP Group                                          |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | https://www.php.net/license/3_01.txt                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
*/

/*
 This is a WebAssembly userland setjmp/longjmp implementation based on Binaryen's Asyncify.
 Inspired by Alon Zakai's snippet released under the MIT License:
 * https://github.com/kripken/talks/blob/991fb1e4b6d7e4b0ea6b3e462d5643f11d422771/jmp.c

 WebAssembly doesn't have context-switching mechanism for now, so emulate it by Asyncify,
 which transforms WebAssembly binary to unwind/rewind the execution point and store/restore
 locals.

 The basic concept of this implementation is:
 1. setjmp captures the current execution context by unwinding to the root frame, then immediately
    rewind to the setjmp call using the captured context. The context is saved in jmp_buf.
 2. longjmp unwinds to the root frame and rewinds to a setjmp call re-using a passed jmp_buf.

 This implementation also supports switching context across different call stack (non-standard)

 This approach is good at behavior reproducibility and self-containedness compared to Emscripten's
 JS exception approach. However this is super expensive because Asyncify inserts many glue code to
 control execution point in userland.

 This implementation will be replaced with future stack-switching feature.
 */
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "polyfill_setjmp/asyncify.h"
#include "polyfill_setjmp/setjmp.h"

// #define PHP_WASM_ENABLE_DEBUG_LOG 1

#ifdef PHP_WASM_ENABLE_DEBUG_LOG
# include <stdio.h>
# define PHP_WASM_DEBUG_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
# define PHP_WASM_DEBUG_LOG(...)
#endif

enum php_wasm_jmp_buf_state {
    // Initial state
    JMP_BUF_STATE_INITIALIZED = 0,
    // Unwinding to the root or rewinding to the setjmp call
    // to capture the current execution context
    JMP_BUF_STATE_CAPTURING   = 1,
    // Ready for longjmp
    JMP_BUF_STATE_CAPTURED    = 2,
    // Unwinding to the root or rewinding to the setjmp call
    // to restore the execution context
    JMP_BUF_STATE_RETURNING   = 3,
};

void
async_buf_init(struct __php_wasm_asyncify_jmp_buf* buf)
{
    buf->top = &buf->buffer[0];
    buf->end = &buf->buffer[WASM_SETJMP_STACK_BUFFER_SIZE];
}

// Global unwinding/rewinding jmpbuf state
static php_wasm_jmp_buf *_php_wasm_active_jmpbuf;
void *php_asyncify_unwind_buf;

__attribute__((noinline))
int
_php_wasm_setjmp_internal(php_wasm_jmp_buf *env)
{
    PHP_WASM_DEBUG_LOG("[%s] env = %p, env->state = %d, _php_wasm_active_jmpbuf = %p\n", __func__, env, env->state, _php_wasm_active_jmpbuf);
    switch (env->state) {
    case JMP_BUF_STATE_INITIALIZED: {
        PHP_WASM_DEBUG_LOG("[%s] JMP_BUF_STATE_INITIALIZED\n", __func__);
        env->state = JMP_BUF_STATE_CAPTURING;
        env->payload = 0;
        _php_wasm_active_jmpbuf = env;
        async_buf_init(&env->setjmp_buf);
        asyncify_start_unwind(&env->setjmp_buf);
        return -1; // return a dummy value
    }
    case JMP_BUF_STATE_CAPTURING: {
        asyncify_stop_rewind();
        PHP_WASM_DEBUG_LOG("[%s] JMP_BUF_STATE_CAPTURING\n", __func__);
        env->state = JMP_BUF_STATE_CAPTURED;
        _php_wasm_active_jmpbuf = NULL;
        return 0;
    }
    case JMP_BUF_STATE_RETURNING: {
        asyncify_stop_rewind();
        PHP_WASM_DEBUG_LOG("[%s] JMP_BUF_STATE_RETURNING\n", __func__);
        env->state = JMP_BUF_STATE_CAPTURED;
        _php_wasm_active_jmpbuf = NULL;
        return env->payload;
    }
    default:
        assert(0 && "unexpected state");
    }
    return 0;
}

void
_php_wasm_longjmp(php_wasm_jmp_buf* env, int value)
{
    PHP_WASM_DEBUG_LOG("[%s] env = %p, env->state = %d, value = %d\n", __func__, env, env->state, value);
    assert(env->state == JMP_BUF_STATE_CAPTURED);
    assert(value != 0);
    env->state = JMP_BUF_STATE_RETURNING;
    env->payload = value;
    _php_wasm_active_jmpbuf = env;
    async_buf_init(&env->longjmp_buf);
    asyncify_start_unwind(&env->longjmp_buf);
}

void *
php_wasm_handle_jmp_unwind(void)
{
    PHP_WASM_DEBUG_LOG("[%s] _php_wasm_active_jmpbuf = %p\n", __func__, _php_wasm_active_jmpbuf);
    if (!_php_wasm_active_jmpbuf) {
        return NULL;
    }

    switch (_php_wasm_active_jmpbuf->state) {
    case JMP_BUF_STATE_CAPTURING: {
        PHP_WASM_DEBUG_LOG("[%s] JMP_BUF_STATE_CAPTURING\n", __func__);
        // save the captured Asyncify stack top
        _php_wasm_active_jmpbuf->dst_buf_top = _php_wasm_active_jmpbuf->setjmp_buf.top;
        break;
    }
    case JMP_BUF_STATE_RETURNING: {
        PHP_WASM_DEBUG_LOG("[%s] JMP_BUF_STATE_RETURNING\n", __func__);
        // restore the saved Asyncify stack top
        _php_wasm_active_jmpbuf->setjmp_buf.top = _php_wasm_active_jmpbuf->dst_buf_top;
        break;
    }
    default:
        assert(0 && "unexpected state");
    }
    return &_php_wasm_active_jmpbuf->setjmp_buf;
}
