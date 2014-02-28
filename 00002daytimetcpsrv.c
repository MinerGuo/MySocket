/*
 * 00002daytimetcpsrv.c
 *
 *  Created on: Dec 26, 2012
 *      Author: mguo
 */
#include "lib/include/unp.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>

int main(int argc, char ** args) {
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY );

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	listen(listenfd, LISTEN1024);

	//无限循环，始终监听
	for (;;) {
		connfd = accept(listenfd, NULL, NULL);
		ticks = time(NULL );
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(connfd, buff, sizeof(buff));
		close(connfd);
	}
}

