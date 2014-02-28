/*
 * sigprocmask3.c
 *
 *  Created on: 2013-5-26
 *      Author: miner
 */
#include "include/sigprocmask.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void sig_int(int signo);

int main(int argc, char ** argv) {
	sigset_t newmask, oldmask, waitmask;

	pr_mask("program start:");

	if (signal(SIGINT, sig_int) == SIG_ERR ) {
		printf("set signal int catch function error!\n");
		exit(-1);
	}

	sigemptyset(&newmask);
	sigemptyset(&oldmask);
	sigemptyset(&waitmask);

	sigaddset(&waitmask, SIGUSR1);
	sigaddset(&newmask, SIGINT);


	//阻塞SIGINT信号，保存当前信号集
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		printf("set signal sigprocmask error!\n");
		exit(-1);
	}

	pr_mask("in critcal region:");

	//暂停，允许除了SIGUSR1信号的所有信号，这时，即使上面block了SIGINT信号，依然能够发送该信号给进程
	//并且进入处理函数，然后sigsuspend返回，返回后，信号集恢复原来值，SIGINT又再次被阻塞
	if (sigsuspend(&waitmask) != -1) {
		printf("set signal sigsuspend error!\n");
		exit(-1);
	}

	sleep(5);
	//这里已经收不到sigint信号了

	pr_mask("after return sigsuspend:");

	sigprocmask(SIG_SETMASK, &oldmask, NULL);

	//这里又能够收到之前阻塞的sigint，并进入到处理函数中

	pr_mask("program exit:");

	exit(0);
}

void sig_int(int signo){
	pr_mask("cat signal in sig_int ：");
}

/**
 *
 * program start:
in critcal region:SIGINT
^Ccat signal in sig_int ：SIGINT SIGUSR1
^Cafter return sigsuspend:SIGINT
cat signal in sig_int ：SIGINT
program exit:
 *
 */
