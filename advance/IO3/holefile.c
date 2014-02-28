/*
 * holefile.c
 *
 *  Created on: 2013-4-27
 *      Author: miner
 */

#include "include/holefile.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void createHoleFile() {
	int fd = 0;
	char * buffer1 = "abcdefghij";
	char * buffer2 = "ABCDEFGHIJ";

	fd = creat("file.hole", S_IRWXU | S_IRUSR);
	if (fd < 0) {
		printf("creat file error!\n");
		_exit(-1);
	}

	if (write(fd, buffer1, 10) != 10) {
		printf("buffer1 write error!\n");
		_exit(-1);
	}

	if (lseek(fd, 1024 * 1024, SEEK_SET) == -1) {
		printf("lseek 1024*1024 error!\n");
		_exit(-1);
	}

	if (write(fd, buffer2, 10) != 10) {
		printf("buffer2 write error!\n");
		_exit(-1);
	}

	return;
}

int main(int argc, char ** argv) {
	createHoleFile();
	return 1;
}

/*ls -l 发现file.hole大小为1048586
-rwx------  1 miner 1048586  4月 27 22:14 file.hole
-rwxrwxr-x  1 miner    8952  4月 27 22:14 holefile

 od -c file.hole 以字符查看文件内容发现中间有好多的0，这个就是所谓的空洞：
 0000000   a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
4000000   A   B   C   D   E   F   G   H   I   J

 ls -ls 发现file.hole占用8个磁盘块，而比它体积小的holefile却占用了12个磁盘块
  8 -rwx------ 1 miner miner 1048586  4月 27 22:14 file.hole
12 -rwxrwxr-x 1 miner miner    8952  4月 27 22:14 holefile

 说明空洞文件虽然显示文件长度很大，但不占用磁盘空间



*/

