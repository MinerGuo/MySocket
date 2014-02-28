/*
 * listen.h
 *
 *  Created on: Jan 10, 2013
 *      Author: mguo
 */

#ifndef LISTEN_H_
#define LISTEN_H_

#include "unp.h"
#include <stdlib.h>

void Listen(int fd,int backlog){
	char * ptr;
	if( (ptr = getenv("LISTENQ")) != NULL ){
		backlog = atoi(ptr);
	}
	if(listen(fd,backlog) < 0){
		err_quit("listen error!");
	}
}


#endif /* LISTEN_H_ */
