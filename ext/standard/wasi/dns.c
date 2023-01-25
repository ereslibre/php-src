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

#include "php.h"
#include "php_network.h"

#include <netinet/in.h>

/* {{{ Get the Internet host name corresponding to a given IP address */
PHP_FUNCTION(gethostbyaddr)
{
	char *addr;
	size_t addr_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH(addr, addr_len)
	ZEND_PARSE_PARAMETERS_END();

  // On WASI environments, return the given IP address, without resolving its name
  RETURN_STR(zend_string_init(addr, sizeof(addr), 0));
}
/* }}} */

/* {{{ Get the IP address corresponding to a given Internet host name */
PHP_FUNCTION(gethostbyname)
{
	char *hostname;
	size_t hostname_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_PATH(hostname, hostname_len)
	ZEND_PARSE_PARAMETERS_END();

	if (hostname_len > MAXFQDNLEN) {
		php_error_docref(NULL, E_WARNING, "Host name cannot be longer than %d characters", MAXFQDNLEN);
		RETURN_STRINGL(hostname, hostname_len);
	}

  // On WASI environments, return the given hostname, without resolving its IP addresses
  RETURN_STR(zend_string_init(hostname, sizeof(hostname), 0));
}
/* }}} */

/* {{{ Return a list of IP addresses that a given hostname resolves to. */
PHP_FUNCTION(gethostbynamel) /* {{{ */
{
	RETURN_FALSE;
}
/* }}} */
