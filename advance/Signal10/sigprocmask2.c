/*
 * sigprocmask2.c
 *
 *  Created on: 2013-5-26
 *      Author: miner
 */
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void sig_quit(int signo);

int main(int argc, char ** argv) {
	sigset_t newmask, oldmask, pendmask;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR ) {
		printf("can't catch SIGQUIT!\n");
		exit(-1);
	}

	//将sigquit添加到新的 信号集 中，并且将已有的信号集保存到oldmask中
	sigemptyset(&newmask);
	sigemptyset(&pendmask);
	sigaddset(&newmask, SIGQUIT);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		printf("can't block SIGQUIT!\n");
		exit(-1);
	} else {
		printf("SIGQUIT is blocked!\n");
	}
	//到此时，系统将阻塞QUIT信号传递到进程中。

	sleep(5);

	//使用sigpending函数，将所有已发生但阻塞的信号，放入到给定的参数指定的信号集中
	//注意，这里是已发生但阻塞，如果压根就没有发生，pendmask中就不会有SIGQUIT成员
	if (sigpending(&pendmask) < 0) {
		printf("sigpending error!\n");
		exit(-1);
	} else {
		printf("SIGQUIT is pending !\n");
	}


	if (sigismember(&pendmask, SIGQUIT)) {
		printf("SIGQUIT is a member of pendmask sigset!\n");
	} else {
		printf("SIGQUIT is not a member of pendmask sigset!\n");
	}

	//恢复原来的信号，这里SIG_QUIT将不再阻塞
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL ) < 0) {
		printf("unblock SIG_QUIT error!\n");
		exit(-1);
	} else {
		printf("SIGQUIT is unblock now!\n");
	}

	sleep(5);

	exit(0);
}

void sig_quit(int signo) {
	printf("catch sig quit and processing it!\n");
	//设置回默认动作【程序终止，产生core文件】
	if (signal(SIGQUIT, SIG_DFL ) == SIG_ERR ) {
		printf("can't reset SIGQUIT!\n");
	}
}




/*
 * SIGQUIT is blocked!
^\^\^\^\^\SIGQUIT is pending !
SIGQUIT is a member of pendmask sigset!
catch sig quit and processing it!
SIGQUIT is unblock now!
^\退出 (核心已转储)
 *
 *
 *
 *
 * SIGQUIT is blocked!
SIGQUIT is pending !
SIGQUIT is not a member of pendmask sigset!
SIGQUIT is unblock now!
 *
 *
 */
