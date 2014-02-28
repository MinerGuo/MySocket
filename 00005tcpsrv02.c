/*
 * 00005tcpsrv01.c
 *
 *  Created on: 2013-1-13
 *      Author: root
 *      该版本处理了处理请求任务的子进程在退出时，使用信号捕获和waitpid来避免僵尸进程的问题
 *      同时解决了慢调用accept函数遇见被信号中断后重启，errno=EINTR时的问题，从而使它能够继续accept
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
#include <signal.h>
#include <sys/wait.h>

void str_echo(int sockfd);

void signal_ign_process(int signo) {
	pid_t pid;
	int stat;
	while (pid = waitpid(-1, &stat, WNOHANG) > 0) {
		err_log("child %d terminated!\n", pid);
	}
	return;
}

int main(int argc, char ** argv) {
	int sockfd, connfd;
	struct sockaddr_in serv_addr, clie_addr;
	int client_len;
	pid_t child_pid;

	//注册信号
	signal(SIGCHLD, signal_ign_process);

	//协议族及数据类型
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

	//启动监听
	if (listen(sockfd, 10) != 0) {
		err_quit("listen error%s", strerror(errno));
	}

	for (;;) {
		client_len = sizeof(clie_addr);
		if ((connfd = accept(sockfd, (struct sockaddr *) &clie_addr, &client_len)) < 0) {
			if (errno == EINTR) {
				continue; //back to for
			} else {
				//accept and exit
				err_quit("accept error%s", strerror(errno));
			}
		}
		if ((child_pid = fork()) == 0) {
			//虽然可以在exit时自动关闭，但是我们希望他能够尽早的关闭，已返回给父进程继续使用
			close(sockfd);
			str_echo(connfd);
			//这个close是无所谓的，因为在exit时会自动关闭
			close(connfd);
			exit(0);
		}
		//这个也是必须的
		close(connfd);
	}
	//这个是可选的
	close(sockfd);
}

/**
 * 接收用户用空格分隔两个数字，相加后返回给客户端
 */
void str_echo(int sockfd) {
	ssize_t n;
	char buf[MY_BUFFER_LEN];

	long value1, value2;
	while ((n = read(sockfd, buf, MY_BUFFER_LEN))) {
		if (n > 0) {
			if (sscanf(buf, "%ld %ld", &value1, &value2) == 2) {
				snprintf(buf, sizeof(buf), "%ld\n", (value1 + value2));
			} else {
				snprintf(buf, sizeof(buf), "input is not number\n");
			}
			n = strlen(buf);
			write(sockfd, buf, n);
		} else if (n < 0 && errno == EINTR) {
			continue;
		} else if (n < 0) {
			err_quit("read error%s", strerror(errno));
		} else {
			break;
		}
	}
}

