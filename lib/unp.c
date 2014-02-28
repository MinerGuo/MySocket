/*
 * unp.c
 *
 *  Created on: Dec 25, 2012
 *      Author: mguo
 */
#include "include/unp.h"
#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>

static void err_doit(int errnoflag, int level, const char *fmt, va_list ap);


void err_quit(const char * fmt, ...) {
	va_list ap;
	va_start(ap,fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	exit(1);
}

void err_log(const char * fmt, ...) {
	va_list ap;
	va_start(ap,fmt);
	err_doit(0, LOG_ERR, fmt, ap);\
}




static void err_doit(int errnoflag, int level, const char *fmt, va_list ap) {
	int errno_save, n;
	char buf[MAXLINE + 1];
	errno_save = errno;
	vsnprintf(buf, MAXLINE, fmt, ap);
	n = strlen(buf);
	if (errnoflag) {
		vsnprintf(buf + n, MAXLINE, fmt, ap);
	}
	strcat(buf, "\n");

	fflush(stdout);
	fputs(buf, stderr);
	fflush(stderr);

	return;
}
