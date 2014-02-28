/*
 * sigprocmask1.c
 *
 *  Created on: 2013-5-25
 *      Author: miner
 */

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include<stdio.h>
#include "include/sigprocmask.h"



int main(int argc, char ** argv) {
	pr_mask_test("sigset : ");
	exit(1);
}
