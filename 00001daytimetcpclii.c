/*
 * daytimetcpclii.c
 *
 *  Created on: Dec 25, 2012
 *      Author: mguo
 */

#include "lib/include/unp.h"
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <errno.h>

//http://tf.nist.gov/tf-cgi/servers.cgi
//telnet 206.168.112.96  13
int main(int argc, char ** argv) {
	int sockfd, n;
	char recvline[MAXLINE + 1];
	//网际套接字地址
	struct sockaddr_in servaddr;

	if (argc != 2) {
		err_quit("arc is not %d!!", argc);
	}

	//创建一个网际(IPV4)，字节流类型的本地套接字
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		err_quit("socket error");
	}

	//网际套接字地址清0
	bzero(&servaddr, sizeof(servaddr));
	//网际套接字地址族(IPV4)
	servaddr.sin_family = AF_INET;
	//端口号(主机到网络短整型)
	servaddr.sin_port = htons(13);
	err_log("htons(13)=%d",htons(13));

	//设置网际套接字的IP地址(呈现形式到数值的转换)
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		err_quit("inet_pton error for %s!!", argv[1]);
	}

	//本地套接字连接上网络套接字
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		err_quit("connect error %s", strerror(errno));
	}

	//获取数据(为0时表示网际套接字关闭连接)
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		if (fputs(recvline, stdout) == EOF) {
			err_quit("fputs error");
		}
	}
	if (n < 0) {
		err_quit("read error");
	}
	err_quit("exit nomally");
}
