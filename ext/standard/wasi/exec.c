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

PHP_MINIT_FUNCTION(exec)
{
	return SUCCESS;
}

PHP_FUNCTION(exec)
{
	RETURN_FALSE;
}

PHP_FUNCTION(system)
{
	RETURN_FALSE;
}

PHP_FUNCTION(passthru)
{
	RETURN_FALSE;
}

PHP_FUNCTION(escapeshellarg)
{
	RETURN_FALSE;
}

PHP_FUNCTION(escapeshellcmd)
{
	RETURN_FALSE;
}

PHP_FUNCTION(shell_exec)
{
	RETURN_FALSE;
}

PHPAPI int php_exec(int type, const char *cmd, zval *array, zval *return_value)
{
	RETVAL_FALSE;
  return -1;
}
