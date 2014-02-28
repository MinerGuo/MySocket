/*
 * fork2.c
 *
 *  Created on: 2013-4-20
 *      Author: miner
 */

#include "include/fork2.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void test_fork2(){
	pid_t pid;
	pid = fork();
	if(pid < 0){
		//fork error
		printf("fork failed!");
		return;
	}else if(pid == 0){
		//child
		execlp("/bin/ls","ls",NULL);

	}else{
		//parent
		wait(NULL);
		printf("child completed,cid is %d,\n",pid);
	}
}
