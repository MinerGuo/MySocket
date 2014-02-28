/*
 * read.h
 *
 *  Created on: Jan 10, 2013
 *      Author: mguo
 */

#ifndef READ_H_
#define READ_H_

#include "unp.h"
#include <errno.h>

ssize_t readn(int fd,void * ptr,ssize_t n);
ssize_t writen(int fd,const void * ptr,size_t n);
ssize_t readline(int fd,void * ptr,ssize_t max_len);

/**
 * read n bytes from fd
 * note:
 * 		maybe the actual read bytes count is lower than n
 * 		when any  error then return -1
 */
ssize_t readn(int fd, void * ptr, ssize_t n) {
	ssize_t n_read, n_left;
	n_read = 0;
	n_left = n;
	char * buf;
	buf = (char *) ptr;
	while (n_left > 0) {
		if ((n_read = read(fd, buf, n_left)) < 0) {
			if (errno == EINTR) {			//single interrupted,maybe buffer is full
				n_read = 0;
			} else {
				return -1;
			}
		}
		if (n_read == 0) {
			break;	//read end
		}
		n_left = n_left - n_read;
		buf = buf + n_read;
	}
	return n - n_left;
}

ssize_t writen(int fd, const void * ptr, size_t n) {
	ssize_t n_write, n_left;
	n_left = n;
	char * buf = (char *) ptr;
	while (n_left > 0) {
		if ((n_write = write(fd, buf, n)) <= 0) {
			if (n_write < 0 && errno == EINTR) {		//single interrupted,maybe buffer is full
				n_write = 0;
			} else {
				return -1;
			}
		}
		n_left = n_left - n_write;
		ptr = ptr + n_write;
	}
	return n;
}

ssize_t readline(int fd, void * ptr, ssize_t max_len) {
	size_t n_read = 0;
	size_t n_c = 0;
	char * buf = (char *) ptr;
	while (n_read < max_len) {
		if ((n_c = read(fd, buf, 1)) == 1) {
			buf++;
			n_read++;
			if (*(buf - 1) == '\n') {
				break;
			}
		} else if (n_c == 0) {
			*buf = '\0';
			return n_read;
		} else {
			if (errno == EINTR) {
				continue;
			} else {
				return -1;
			}
		}
	}
	*buf = '\0';
	return n_read;
}

#endif /* READ_H_ */
