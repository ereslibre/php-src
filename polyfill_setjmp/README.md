# `setjmp/longjmp` with asyncify

The implementation is taken from the [Ruby
WASM](https://github.com/ruby/ruby/blob/36420068725e5b4b86b3481fa1f82b249a60d928/wasm/README.md)
support library.

This implementation requires a to run the `asyncify` wasm-opt pass on
the resulting PHP binary, so that functions are instrumented allowing
us to emulate `setjmp`/`longjmp` behavior.

wasm-opt -O --asyncify -g --pass-arg=asyncify-ignore-imports -o php-cgi php-cgi
