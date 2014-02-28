/*
 * 00004daytimetcpsrv1.c
 *
 *  Created on: Jan 10, 2013
 *      Author: mguo
 */

#include <stdlib.h>
#include <stdio.h>
#include "lib/include/unp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <strings.h>
#include <time.h>
#include <string.h>
/**
 * 显示客户IP地址和时间端口号的时间获取服务器程序
 */
int main(int argc, char ** argv) {
	int socketfd, connfd;
	socklen_t accept_len;
	struct sockaddr_in server_addr, client_addr;
	char buff[MY_BUFFER_LEN];
	time_t ticks;

	//step1:为了得到一个套接字描述符，
	//我们需要指定协议族(ipv4/ipv6/unix域/路由协议)
	//套接字类型(字节流/数据报/原始套接字)
	//创建手法为调用socket函数
	//这里创建一个套接字描述符文件,AF_INET ipv4协议+字节流
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1) {
		err_quit("socket error%s", strerror(errno));
	}

	//step2:创建本地通信协议，包含网络接口(IP)地址和端口号
	//如果是服务器端，IP地址指的是限定客户端的请求源
	//端口号也可以不指定，如果这样，将由内核来指定一个临时的端口号，为了得到这个端口号，必须调用getsockname来获取
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY ); //0.0.0.0 意味着可以接收来自任何地方的请求
	server_addr.sin_port = htons(7777); //端口号

	//step3:将本地通信协议的地址，与套接字文件描述符绑定在一起
	if (bind(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr))
			!= 0) {
		err_quit("bind error%s", strerror(errno));
	}

	//step4:启动监听,导致socketfd由closed状态转换为listen状态
	//第二个参数指定了，connetion请求的最大队列，最好不要指定为0，因为各个系统的解释不一样
	//如果你不想有任何链接请求进来，可以关闭该套接字
	if (listen(socketfd, 10) != 0) {
		err_quit("listen error%s", strerror(errno));
	}

	for (;;) {
		//step5:从已完成连接的队列头，取出一个已完成链接
		//其返回值是内核生成的一个全新的描述符，用来标致客户的链接
		accept_len = sizeof(client_addr);
		connfd = accept(socketfd, (struct sockaddr *) &server_addr, &accept_len);
		printf("connection from %s,port: %d\n",
				inet_ntop(AF_INET,(struct sockaddr *)  &client_addr.sin_addr, buff, sizeof(buff)),
				ntohs(client_addr.sin_port));
		ticks = time(NULL);
		snprintf(buff,sizeof(buff),"%.24s,connfd is %d.\r\n",ctime(&ticks),connfd);
		write(connfd,buff,strlen(buff));
		close(connfd);

	}
	close(socketfd);

}
