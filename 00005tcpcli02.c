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

int pipe_eror = 0;
void signal_pipe_process(int signo) {
	err_log("catch  sigpipe signal but i still not exit!\n");
	pipe_eror = 1;
	return;
}

int main(int argc, char ** argv) {
	int sockfd;
	struct sockaddr_in cli_addr;

	signal(SIGPIPE, signal_pipe_process);

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

void str_cli(FILE * fp, int sockfd) {
	char sendline[MY_BUFFER_LEN], recvline[MY_BUFFER_LEN];
	int n;
	char output[10];
	while (fgets(sendline, MY_BUFFER_LEN, fp) != NULL ) {

		struct param param;
		struct result result;
		if (sscanf(sendline, "%ld %ld", &(param.arg1), &(param.arg2)) == 2){

		//写二进制数据
		n = write(sockfd, &param, sizeof(param));

		if (n > 0) {

			//读二进制数据
			if (readline(sockfd, &result, sizeof(result)) == 0) {
				err_log("client:server terminated!but i don't exit!\n");
			}
			else{
				snprintf(output, 10, "%ld\n",result.value);
				fputs(output, stdout);
			}
		} else if (n < 0 && pipe_eror ==1) {
			err_log("write content to socket error:pipe_eror,but i don't exit,errno = %s!\n",strerror(errno));
			continue;
		} else if (n < 0 && errno == EINTR) {
			err_log("write content to socket error:errno == EINTR!\n");
			continue;
		} else if (n < 0) {
			err_quit("read error%s", strerror(errno));
		} else {
			break;
		}
		}else{
			err_log("input is not two numbers!\n");
						continue;
		}

	}
}

