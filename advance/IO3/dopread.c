/*
 * pread.c
 *
 *  Created on: 2013-4-27
 *      Author: miner
 */

#include "include/dopread.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void do_pread() {
	char buffer[32];
	memset(buffer, '\0', 32);
	char * wirte_buffer = "this is the end!";
	int fd = open("file.hole", O_RDWR);
	if (fd == -1) {
		printf("open file.hole fail\n");
		_exit(-1);
	}
	ssize_t read_count = pread(fd, buffer, 31, 0);
	if (read_count == -1) {
		printf("read file.hole fail\n");
		_exit(-1);
	}
	printf("read content is %s\n", buffer);

	ssize_t write_count = pwrite(fd, wirte_buffer, 16, 12);

	if (write_count == -1) {
		printf("write file.hole fail\n");
		_exit(-1);
	}
	printf("write success\n");

	memset(buffer, '\0', 32);
	read_count = pread(fd, buffer, 31, 0);
	if (read_count == -1) {
		printf("after write,read file.hole fail\n");
		_exit(-1);
	}
	printf("after write,read content is %s\n", buffer);

	return;
}

int main(int argc, char ** argv) {
	do_pread();
	return 1;
}

/*
 * ls -la
 * -rwx------  1 miner miner 1048586  4月 27 22:14 file.hole
 *
od -c file.hole
0000000   a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
4000000   A   B   C   D   E   F   G   H   I   J
4000012
 *
 *
 *
 *执行程序之后：
 -rwx------  1 miner miner 1048586  4月 27 22:59 file.hole

od -c file.hole
0000000   a   b   c   d   e   f   g   h   i   j  \0  \0   t   h   i   s
0000020       i   s       t   h   e       e   n   d   !  \0  \0  \0  \0
0000040  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
4000000   A   B   C   D   E   F   G   H   I   J

 *
 *
 */
