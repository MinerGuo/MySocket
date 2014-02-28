/*
 * readbuf.h
 *
 *  Created on: Jan 10, 2013
 *      Author: mguo
 */

#ifndef READBUF_H_
#define READBUF_H_

#include "unp.h"
#include <errno.h>

static ssize_t read_cnt;
static char* read_ptr;
static char read_buf[MY_BUFFER_LEN]; //1k

/**
 * 使用局部静态变量来实现自己的缓冲区
 */
ssize_t my_read(int fd, char * ptr) {
	if (read_cnt <= 0) {
		//缓冲区无内容，则需要从fd中读取
		while ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR) {
				continue;
			}
			return -1;
		}
		if (read_cnt == 0) {
			return 0;
		}
		read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr;
	read_ptr++;

	return 1;
}

/**
 * 获取一行，返回实际读取的字符数
 */
ssize_t my_readline(int fd, char *vptr, size_t maxlen) {
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ((rc = (my_read(fd, &c))) == 1) {
			*ptr = c;
			ptr++;
			if (c == '\n') {
				break;
			} else if (rc == 0) {
				*ptr = 0;
				return n - 1;
			} else {
				continue;
			}
		}
	}
	*ptr = '\0';
	return n;

}

/**
 * 获取缓冲区里的内容
 */
ssize_t readlinebuf(void ** vptrptr){
	if(read_cnt){
		*vptrptr = read_ptr;
	}
	return read_cnt;
}
#endif /* READBUF_H_ */
