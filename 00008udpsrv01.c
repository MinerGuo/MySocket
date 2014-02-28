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



int dg_echo(int sockfd, struct sockaddr * clie_addr, socklen_t clilen){
	ssize_t n,r;
	char buffer[MY_BUFFER_LEN];
	socklen_t cli_len_t;
	for(;;){
		cli_len_t = clilen;
		n = recvfrom(sockfd, buffer, MY_BUFFER_LEN, 0,clie_addr, &cli_len_t);
		if(n >0){
			r = sendto(sockfd, buffer, n, 0,clie_addr,  cli_len_t);
			err_log("sendto write successfully:  %d\n", r);
		}
	}
}

int main(int argc, char ** argv) {
	int sockfd, connfd;
	struct sockaddr_in serv_addr, clie_addr;



	//协议族及数据类型
	//SOCK_DGRAM UDP是用这个哦
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		err_quit("socket error%s", strerror(errno));
	}

	//创建本地协议地址
	bzero(&serv_addr, sizeof(&serv_addr));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(7777);

	//绑定本地协议地址到sock文件描述符
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		err_quit("bind error%s", strerror(errno));
	}

	dg_echo(sockfd,(struct sockaddr *)&clie_addr,sizeof(clie_addr));

}

