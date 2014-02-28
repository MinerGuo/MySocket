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
#include "lib/include/readbuf.h"

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen) {
	char portstr[8];
	static char str[128]; //Unix domain is largest
	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in *sin = (struct sockaddr_in *) sa;
		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL )
			return (NULL );
		if (ntohs(sin->sin_port) != 0) {
			snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
		}
		return (str);
	}
	}

	int dg_cli(int sockfd, struct sockaddr * servaddr, socklen_t ser_len) {
		ssize_t n, r;
		char sendline[MY_BUFFER_LEN], recvline[MY_BUFFER_LEN + 1];
		socklen_t ser_len_t;

		struct sockaddr * preply_addr;
		preply_addr = malloc(ser_len);

		while (my_readline(fileno(stdin),sendline, MY_BUFFER_LEN) > 0) {

			ser_len_t = ser_len;

			sendto(sockfd, sendline, strlen(sendline), 0,servaddr, ser_len_t);

			//后两个NULL告诉内核我们并不关心应答数据报由谁发送
				n = recvfrom(sockfd, recvline, MY_BUFFER_LEN, 0, preply_addr, &ser_len_t);

				if(ser_len_t!=ser_len || memcmp(servaddr,preply_addr)!=0 ) {
					err_log("reply from %s (ignored) \n!!", sock_ntop(preply_addr, ser_len));
					continue;
				}

				if (n > 0) {
					recvline[n] = 0;
					fputs(recvline, stdout);
				}
			}
		}

	int main(int argc, char ** argv) {
		int sockfd, connfd;
		struct sockaddr_in serv_addr;

		//网际套接字地址
		struct sockaddr_in servaddr;

		if (argc != 3) {
			err_quit("arc is not %d!!", argc);
		}

		//网际套接字地址清0
		bzero(&servaddr, sizeof(servaddr));
		//网际套接字地址族(IPV4)
		servaddr.sin_family = AF_INET;
		//端口号(主机到网络短整型)
		servaddr.sin_port = htons(atoi(argv[2]));

		//设置网际套接字的IP地址(呈现形式到数值的转换)
		if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
			err_quit("inet_pton error for %s!!", argv[1]);
		}

		//协议族及数据类型
		//SOCK_DGRAM UDP是用这个哦
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd == -1) {
			err_quit("socket error%s", strerror(errno));
		}
		dg_cli(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	}

