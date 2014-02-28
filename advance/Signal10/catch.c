/*
 * catch.c
 *
 *  Created on: 2013-5-16
 *      Author: miner
 */

#include "include/catch.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	if (signal(SIGUSR1, signal_usr_process) == SIG_ERR ) {
		printf("can't catch signal SIGUSR1\n");
		exit(-1);
	}
	if (signal(SIGUSR2, signal_usr_process) == SIG_ERR ) {
		printf("can't catch signal SIGUSR2\n");
		exit(-1);
	}
	for(;;) {
		pause();
	}
	exit(1);
}

void signal_usr_process(int signo){
	if(signo == SIGUSR1){
		printf("received signal SIGUSR1\n");
	}else if(signo == SIGUSR2){
		printf("received signal SIGUSR2\n");
	}else{
		printf("received signal %d\n",signo);
	}
}

/*
 * kill -USR1 2903
 * kill -USR2 2903
 * kill 2903
 */
