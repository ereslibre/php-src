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

#ifndef PHP_WASM_SUPPORT_ASYNCIFY_H
#define PHP_WASM_SUPPORT_ASYNCIFY_H

__attribute__((import_module("asyncify"), import_name("start_unwind")))
void asyncify_start_unwind(void *buf);
#define asyncify_start_unwind(buf) do {  \
    extern void *php_asyncify_unwind_buf; \
    php_asyncify_unwind_buf = (buf);      \
    asyncify_start_unwind((buf));        \
  } while (0)
__attribute__((import_module("asyncify"), import_name("stop_unwind")))
void asyncify_stop_unwind(void);
#define asyncify_stop_unwind() do {      \
    extern void *php_asyncify_unwind_buf; \
    php_asyncify_unwind_buf = NULL;       \
    asyncify_stop_unwind();              \
  } while (0)
__attribute__((import_module("asyncify"), import_name("start_rewind")))
void asyncify_start_rewind(void *buf);
__attribute__((import_module("asyncify"), import_name("stop_rewind")))
void asyncify_stop_rewind(void);

#endif
