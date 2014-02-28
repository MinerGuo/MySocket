/*
 * 00003byteorder.c
 *
 *  Created on: Dec 31, 2012
 *      Author: mguo
 */
#include<stdlib.h>
#include<stdio.h>

struct byte_order{
	short num;
	char *  c;
};


int main(int argc, char ** args) {
	struct byte_order bo;
	bo.num=0x0102;
	bo.c = (char *)&bo.num;

	if(bo.c[0] == 1 && bo.c[1] == 2){
		printf("high order");
	}else if(bo.c[0] == 2 && bo.c[1] == 1){
		printf("lower order");
	}else{
		printf("unkown order");
	}

	exit(0);
}

