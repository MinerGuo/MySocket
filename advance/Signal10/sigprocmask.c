/*
 * sigprocmask.c
 *
 *  Created on: 2013-5-26
 *      Author: miner
 */
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include<stdio.h>
#include "include/sigprocmask.h"


void pr_mask(const char * str) {
	sigset_t sigset;
	int errno_save;

	sigemptyset(&sigset);
	errno_save = errno;

	if (sigprocmask(0,NULL, &sigset) < 0) {
		printf("sigprocmask error!\n");
		exit(-1);
	}


	printf("%s", str);

	if (sigismember(&sigset, SIGINT)) {
		printf("SIGINT ");
	}
	if (sigismember(&sigset, SIGQUIT)) {
		printf("SIGQUIT ");
	}
	if (sigismember(&sigset, SIGUSR1)) {
		printf("SIGUSR1 ");
	}
	if (sigismember(&sigset, SIGALRM)) {
		printf("SIGALRM ");
	}

	printf("\n");

	errno  = errno_save;
}

void pr_mask_test(const char * str) {
	sigset_t sigset,sigold;
	int errno_save;

	sigemptyset(&sigset);
	errno_save = errno;

	if (sigprocmask(0, &sigset, &sigold) < 0) {
		printf("sigprocmask error!\n");
		exit(-1);
	}

	//将SIGINT信号屏蔽掉，就是让系统阻塞SIGINT信号发送给进程
	sigaddset(&sigset,SIGINT);

	printf("%s", str);

	if (sigismember(&sigset, SIGINT)) {
		printf("SIGINT ");
	}
	if (sigismember(&sigset, SIGQUIT)) {
		printf("SIGQUIT ");
	}
	if (sigismember(&sigset, SIGUSR1)) {
		printf("SIGUSR1 ");
	}
	if (sigismember(&sigset, SIGALRM)) {
		printf("SIGALRM ");
	}
	//恢复原先保存的屏蔽信号集合。。根据打印结果显示，系统默认不会阻塞任何信号
	sigprocmask(SIG_SETMASK,&sigold,NULL);

	printf("\n");
}
