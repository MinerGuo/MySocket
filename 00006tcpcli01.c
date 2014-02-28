/*
 * 00005tcpcli01.c
 *
 *  Created on: 2013-1-13
 *      Author: root
 */
#include <stdlib.h>
#include <stdio.h>
#include "lib/include/unp.h"
#include "lib/include/read.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <strings.h>
#include <time.h>
#include <string.h>
#include <signal.h>

void str_cli(FILE * pf, int sockfd);

int main(int argc, char ** argv) {
	int sockfd;
	struct sockaddr_in cli_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		err_quit("socket error%s", strerror(errno));
	}

	bzero(&cli_addr, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(7777);
	inet_pton(AF_INET, "localhost", (struct sockaddr *) &cli_addr.sin_addr);

	if (connect(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) == -1) {
		err_quit("connect error%s", strerror(errno));
	}

	str_cli(stdin,sockfd);

	exit(0);
}

/**
 * 使用select模式，对用户输入和socket两个文件描述符进行监听，
 * 从而避免任何一个阻塞时，无法接受另外一个的就绪状态数据
 */
void str_cli(FILE * fp, int sockfd) {
	char sendline[MY_BUFFER_LEN], recvline[MY_BUFFER_LEN];
	int n;
	char output[10];
	int max_fd = max(fileno(fp), sockfd) + 1;
	fd_set fd_set_watch_list;

	for (;;) {
		FD_ZERO(&fd_set_watch_list);
		FD_SET(fileno(fp), &fd_set_watch_list);
		FD_SET(sockfd, &fd_set_watch_list);

		//这里我们只关心两个套接字都已经准备好了读
		select(max_fd, &fd_set_watch_list, NULL, NULL, NULL );

		if (FD_ISSET(sockfd, &fd_set_watch_list)) {
			//服务器端将我们发送过去的数据回射过来了，我们这里可以读了
			if (readline(sockfd, recvline, MY_BUFFER_LEN) == 0) {
				err_quit("client:read socket return EOF and i will exit!\n");
			}

			fputs(recvline, stdout);
		}
		if (FD_ISSET(fileno(fp), &fd_set_watch_list)) {
			//用户已经输入一些数据了
			if (fgets(sendline, MY_BUFFER_LEN, fp) == NULL ) {
				err_quit("client:read user input return EOF and i will exit!\n");
			}
			//将用户输入的数据，写入到服务器端去
			write(sockfd, sendline, strlen(sendline));
		}
	}

}

