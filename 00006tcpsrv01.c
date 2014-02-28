/**
 * 使用select模式，对已就绪监听描述符和已连接描述符进行监控
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

int pipe_eror = 0;
void signal_pipe_process(int signo) {
	err_log("catch  sigpipe signal but i still not exit!\n");
	pipe_eror = 1;
	return;
}

/**
 * 基本思路，使用一个数组，保存监听描述符和所有已连接的描述符，对这些描述符进行监听；
 * 每新完成一个连接，数组就往后移动一位，并且设置其值为1,
 * 每关闭一个连接，就设置该值为0
 * 初始化为-1
 * 数组长度是进程可打开的最大文件描述符和FD_SETSIZE的最小值
 */
int main(int argc, char ** argv) {
	int sockfd, connfd, nready;
	struct sockaddr_in serv_addr, clie_addr;
	int client_len;
	int t;
	int read_count = 0;
	pid_t child_pid;

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

	int fileset[FD_SETSIZE];
	for (t = 0; t < FD_SETSIZE; t++) {
		fileset[t] = -1;
	}
	int max_fd = sockfd;
	fd_set allset;

	for (;;) {
		FD_ZERO(&allset);
		//,无论如何，监听套接字肯定是要被监听的
		FD_SET(sockfd, &allset);
		for (t = 0; t <= max_fd; t++) {
			if (fileset[t] == 1) {
				FD_SET(t, &allset);
			}
		}

		//still only focus on read
		nready = select(max_fd + 1, &allset, NULL, NULL, NULL );
		if (FD_ISSET(sockfd, &allset)) {
			client_len = sizeof(clie_addr);

			if (max_fd == FD_SETSIZE) {
				err_log("connection pool is full, server can't accept anymore new request!");
			}
			//有新的连接就绪队列了
			else if ((connfd = accept(sockfd, (struct sockaddr *) &clie_addr, &client_len)) < 0) {
				if (errno == EINTR) {
					err_log("accept EINTR error:%s", strerror(errno));
				} else {
					//accept and exit
					err_quit("accept error%s", strerror(errno));
				}
			} else {
				//连接成功，服务器将使用新的connfd描述符来与客户端通信，
				//所以需要将这个套接字加入到fd_set中
				if(connfd > max_fd)
					max_fd = connfd;
				fileset[connfd] = 1;
				FD_SET(connfd, &allset);
				err_log("accept :%d", connfd);
			}

			nready--;
			if (nready == 0)
				continue;
		}
		//依次检查已连接上的描述符列表，看看有没有用户输入，也就是已经准备好给服务器端读的
		for (t = 0; t <= max_fd; t++) {
			if (fileset[t] == 1 && FD_ISSET(t, &allset)) {
				//MY_BUFFER_LEN => 2
				char buf[2];
				do {
					read_count = read(t, buf, 2);
					if (read_count == 0) {
						//客户端关闭了
						close(t);
						//设置为已关闭连接
						fileset[t] = -1;
						FD_CLR(t, &allset);
					} else if (read_count < 0 && errno == EINTR) {
						continue;
					} else {
						write(t, buf, read_count);
					}
					break;
				} while (1);
				nready--;
				if (nready == 0)
					continue;
			}
		}
	}
	//这个是可选的
	close(sockfd);
}

