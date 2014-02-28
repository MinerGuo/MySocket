/*
 * fork.c
 *
 *  Created on: 2013-4-18
 *      Author: miner
 */


#include "include/fork.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int value = 5;
void test_fork(){
	pid_t pid;
	pid = fork();

	if(pid == 0){
		//child
		value += 15;
	}else{
		wait(NULL);
		printf("parent:value = %d \n",value);
		//这里打印的值为5.因为在fork时，子进程复制来父进程的全局变量区间，子进程的改变不会影响到父进程
	}
}
