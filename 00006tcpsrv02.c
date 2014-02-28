/**
 * 使用poll模式，对已就绪监听描述符和已连接描述符进行监控
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
#include <poll.h>

int pipe_eror = 0;
void signal_pipe_process(int signo) {
	err_log("catch  sigpipe signal but i still not exit!\n");
	pipe_eror = 1;
	return;
}

/**
 * 基本思路，使用一个数组，保存监听描述符和所有已连接的描述符，对这些描述符进行监听；
 * 数组长度是进程可打开的最大文件描述符和FD_SETSIZE的最小值
 */
int main(int argc, char ** argv) {
	int sockfd, connfd, nready, maxi;
	struct sockaddr_in serv_addr, clie_addr;
	struct pollfd fds[FD_SETSIZE];
	int client_len;
	int t;
	int read_count = 0;

	signal(SIGPIPE, signal_pipe_process);

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

	for (t = 0; t < FD_SETSIZE; t++) {
		fds[t].fd = -1;
	}

	//无论如何，监听套接字肯定是要被监听的
	fds[0].fd = sockfd;
	fds[0].events = POLLRDNORM; //关心connet可读，也就是有新的连接请求进来
	//指示fds中已经使用了的最大的下标
	maxi = 0;

	for (;;) {

		//第三个参数，超时参数，负数表示永远等待
		nready = poll(fds, maxi + 1, -1);

		if (fds[0].revents & POLLRDNORM) {
			//新的连接进来了
			client_len = sizeof(clie_addr);

			//有新的连接就绪队列了
			if ((connfd = accept(sockfd, (struct sockaddr *) &clie_addr, &client_len)) < 0) {
				if (errno == EINTR) {
					err_log("accept EINTR error:%s", strerror(errno));
				} else {
					//accept and exit
					err_quit("accept error%s", strerror(errno));
				}
			} else {
				//连接成功，服务器将使用新的connfd描述符来与客户端通信，
				//所以需要将这个套接字加入到fds中
				for (t = 1; t < FD_SETSIZE; t++) {
					if (fds[t].fd < 0) {
						fds[t].fd = connfd;
						break;
					}
				}

				if (t == FD_SETSIZE) {
					err_quit("connection pool is full, server can't accept anymore new request!");
				}

				fds[t].events = POLLRDNORM;

				if (t > maxi) {
					maxi = t;
				}

				err_log("accept :t=%d,confd=%d", t, fds[t].fd);
			}

			nready--;
			if (nready == 0)
				continue;
		}

		//依次检查已连接上的描述符列表，看看有没有用户输入，也就是已经准备好给服务器端读的
		for (t = 1; t <= maxi; t++) {

			if (fds[t].fd >= 0 && (fds[t].revents & (POLLRDNORM | POLLERR))) {
				//MY_BUFFER_LEN => 2
				char buf[2];

				read_count = read(fds[t].fd, buf, 2);
				if (read_count <= 0) {
					//error or fin
					close(fds[t].fd);
					//下次可以重用
					fds[t].fd = -1;
				} else {
					write(fds[t].fd, buf, read_count);
				}

				nready--;
				if (nready == 0)
					break;
			}
		}
	}
	//这个是可选的
	close(sockfd);
}

