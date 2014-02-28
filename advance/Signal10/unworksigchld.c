/*
 * unworksigchld.c
 *
 *  Created on: 2013-5-19
 *      Author: miner
 */

#include <signal.h>

void do_sigchld(int signo);
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char ** argv) {
	pid_t pid_child;
	//注册子进程终止信号处理函数
	if (signal(SIGCHLD, do_sigchld) == SIG_ERR ) {
		printf("register function for process child process terminal error!\n");
		exit(-1);
	}

	//创建子进程
	pid_child = fork();
	if (pid_child < 0) {
		printf("fork child process error!\n");
		exit(-1);
	} else if (pid_child == 0) {
		//child
		sleep(2);
	} else {
		//parent wait until some signal generat, include child over siganl(SIGCHLD)
		pause();
	}

	exit(0);
}

void do_sigchld(int signo){
	pid_t pid;
	int status;

	printf("SIGCHLD received!\n");

	//重新注册子进程终止信号处理函数
	if(signal(SIGCHLD, do_sigchld) == SIG_ERR ){
		printf("register function for process child process terminal error!\n");
		exit(-1);
	}
	//等待子进程的状态变化
	if((pid = wait(&status)) < 0){
		printf("fetch child process status error!\n");
		exit(-1);
	}

	printf("pid is %d\n",pid);
}

/*
 * 因为在50行处，进程在检查有没有处于等待的子进程，而我们确实又在等待进程的处理函数中，所以会一直重复执行。
 * 解决此问题的正确方法是，wait在signal之前执行，这样确保了在signal时，当前子进程已经终止，注册其他的子进程结束信号
 *
 * 可实际运行当中，此情况并未发生，do_sigchld只执行了一次而已，奇怪！
 */
