/*
 * unp.h
 *
 *  Created on: Dec 25, 2012
 *      Author: mguo
 */

#ifndef UNP_H_
#define UNP_H_

#include <unistd.h>


#define MAXLINE 100
#define	SA	struct sockaddr
#define	LISTEN1024		1024
#define MY_BUFFER_LEN 1024

void err_quit(const char * fmt,...);
void err_log(const char * fmt,...);

struct result{
	long value;
};

struct param{
	long arg1;
	long arg2;
};

#define min(x,y) ({ typeof(x) _x = (x); typeof(y) _y = (y); (void) (&_x == &_y); _x < _y ? _x : _y; })
#define max(x,y) ({ typeof(x) _x = (x); typeof(y) _y = (y); (void) (&_x == &_y); _x > _y ? _x : _y; })


#endif /* UNP_H_ */
