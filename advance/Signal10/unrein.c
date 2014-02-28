/*
 * unrein.c
 *
 *  Created on: 2013-5-19
 *      Author: miner
 */

#include "include/unrein.h"
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>

void my_alarm(int signo) {
	struct passwd * rootptr;
	printf("signal in my_alarm\n");
	rootptr = getpwnam("miner");
	if (rootptr == NULL ) {
		printf("call getpwnam error!\n");
		exit(-1);
	}
	alarm(1);
}

int main(int argc, char ** argv) {
	struct passwd * ptr;
	signal(SIGALRM, my_alarm);
	alarm(1);

	for (;;) {
		ptr = getpwnam("daemon");
		if (ptr == NULL ) {
			printf("call getpwnam in main function error!\n");
			exit(-1);

		}
		if (strcmp(ptr->pw_name, "daemon") != 0) {
			printf("get name error:%s\n", ptr->pw_name);
			exit(-1);
		}
	}
}


/**
 * 因为getpwnam是不可重入，所以期望该程序运行会报错;
 * 但实际运行中，信号处理程序中的闹钟并没有生效，my_alarm只执行一次，奇怪！
 */
