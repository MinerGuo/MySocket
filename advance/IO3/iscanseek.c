/*
 * iscanseek.c
 *
 *  Created on: 2013-4-27
 *      Author: miner
 */

#include <stdio.h>
#include "include/iscanseek.h"

void iscanseek(int fd){
	if(lseek(fd,0,SEEK_CUR) == -1){
		printf("cannot seek\n");
	}else{
		printf("seek ok!\n");
	}
}

int main(int argc, char ** argv){
	iscanseek(STDIN_FILENO);
	return 1;
}

// ./iscanseek </dev/null
