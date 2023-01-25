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
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_SYSLOG_H
#define PHP_SYSLOG_H

#include "php.h"

#ifdef PHP_WIN32
#include "win32/syslog.h"
#else
#include <php_config.h>
#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif
#endif

#ifdef __wasi__
#define	LOG_EMERG   0 /* system is unusable */
#define	LOG_ALERT   1	/* action must be taken immediately */
#define	LOG_CRIT    2	/* critical conditions */
#define	LOG_ERR     3	/* error conditions */
#define	LOG_WARNING 4	/* warning conditions */
#define	LOG_NOTICE  5	/* normal but significant condition */
#define	LOG_INFO    6	/* informational */
#define	LOG_DEBUG   7	/* debug-level messages */
#endif // __wasi__

/* Syslog filters */
#define PHP_SYSLOG_FILTER_ALL		0
#define PHP_SYSLOG_FILTER_NO_CTRL	1
#define PHP_SYSLOG_FILTER_ASCII		2
#define PHP_SYSLOG_FILTER_RAW		3

BEGIN_EXTERN_C()
PHPAPI void php_syslog_str(int priority, const zend_string* message);
PHPAPI void php_syslog(int, const char *format, ...);
PHPAPI void php_openlog(const char *, int, int);
PHPAPI void php_closelog(void);
END_EXTERN_C()

#endif
