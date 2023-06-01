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
   | Author: Sascha Schumann <sascha@schumann.cx>                         |
   +----------------------------------------------------------------------+
*/

#include "php.h"

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(mail)
{
	char *sendmail_path = INI_STR("sendmail_path");
	php_info_print_table_row(2, "Path to sendmail", sendmail_path);
}
/* }}} */

/* {{{ php_mail */
PHPAPI int php_mail(const char *to, const char *subject, const char *message, const char *headers, const char *extra_cmd)
{
	return 0;
}
/* }}} */
