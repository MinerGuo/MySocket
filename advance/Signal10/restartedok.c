/*
 * restartedok.c
 *
 *  Created on: 2013-6-1
 *      Author: miner
 */

//写任何一个函数时，首先要考虑到它是否是线程安全的
///所谓线程安全，就是多个线程同时执行该函数，是否会有问题，
//解决这个相对比较容易，尽量不用全局对象，不要使用非线程安全函数，包括malloc都是非线程安全的
//第二点是，要保证其是可重入的，因为线程可能会被信号所中断，信号处理完毕后，会重新执行刚刚执行了一半的函数
//这里特别要注意的是，如果函数使用了互斥锁，则当重入时，又要再获取锁，就会导致死锁的发生，解决方案是使用递归锁
//上面说的两个概念一个是线程安全，一个是可重入
//实际测试时，却没有出现错误的结果，机器CPU太少来，并发度跑不起来
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARG_MAX 200

static char envbuf[ARG_MAX];

char ** envrion;
int pthread_exec_count;
int g_index;

pthread_mutex_t env_mutex;
pthread_once_t init_done = PTHREAD_ONCE_INIT;

void thread_init(void);

/*
 * 使用递归锁实现的 线程安全+可重入
 * 使用两种技术：1：返回值由调用者传入 2：使用递归锁
 */
int safe_getenv(const char * name, char * buf, int buflen) {
	int i, len, olen;
	//保证了所有线程，只执行一次thread_init函数
	pthread_once(&init_done, thread_init);
	len = strlen(name);
	//加锁
	pthread_mutex_lock(&env_mutex);

	for (i = 0; envrion[i] != NULL ; i++) {
		if (strncmp(name, envrion[i], len) == 0 && (envrion[i][len] == '=')) {
			olen = strlen(&envrion[i][len + 1]);
			if (olen >= buflen) {
				pthread_mutex_unlock(&env_mutex);
				return -1;
			}
			strcpy(buf, &envrion[i][len + 1]);
			pthread_mutex_unlock(&env_mutex);
			return 0;
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return -1;
}

void thread_init(void) {
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	//递归
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&env_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

char * getenv(const char * name) {
	int i, len;
	len = strlen(name);

	for (i = 0; envrion[i] != NULL ; i++) {
		if (strncmp(name, envrion[i], len) == 0 && (envrion[i][len] == '=')) {
			strcpy(envbuf, &envrion[i][len + 1]);
			return envbuf;
		}
	}
	return NULL ;
}

void * pthread_do(void * arg) {
	int t_i = 0;
	for (; t_i < pthread_exec_count; t_i++) {
		char * value = getenv("HOME");
		printf("%d:	value is %s\n", g_index, value);
		g_index++;
		fflush(NULL );
	}
	return 0;
}

void * pthread_do2(void * arg) {
	int t_i = 0;
	for (; t_i < pthread_exec_count; t_i++) {
		char * value = getenv("SHELL");
		printf("%d:	value is %s\n", g_index, value);
		g_index++;
		fflush(NULL );
	}
	return 0;
}

void * pthread_do3(void * arg) {
	int t_i = 0;
	for (; t_i < pthread_exec_count; t_i++) {
		char value[100];
		int i = safe_getenv("HOME", value, 100);
		if (i >= 0) {
			printf("%d:	value is %s\n", g_index, value);
			g_index++;
		} else {
			printf("get error!\n");
		}
		fflush(NULL );

	}
	return 0;
}

void * pthread_do4(void * arg) {
	int t_i = 0;
	for (; t_i < pthread_exec_count; t_i++) {
		char value[100];
		int i = safe_getenv("SHELL", value, 100);
		if (i >= 0) {
			printf("%d:	value is %s\n", g_index, value);
			g_index++;
		} else {
			printf("get error!\n");
		}
		fflush(NULL );

	}
	return 0;
}

int main2(int argc, char ** argv) {
	envrion = (char **) malloc(sizeof(char *) * 2);
	envrion[0] = "HOME=/home/testuser/haha";
	envrion[1] = "SHELL=/shell";
	envrion[2] = NULL;
	pthread_exec_count = 200;
	g_index = 0;

	pthread_t t_1;
	pthread_t t_2;
	pthread_t t_3;
	pthread_t t_4;

	pthread_create(&t_1, NULL, pthread_do, NULL );
	pthread_create(&t_3, NULL, pthread_do2, NULL );
	pthread_create(&t_2, NULL, pthread_do, NULL );
	pthread_create(&t_4, NULL, pthread_do2, NULL );

	pthread_join(t_1, NULL );
	pthread_join(t_2, NULL );
	pthread_join(t_3, NULL );
	pthread_join(t_4, NULL );

	exit(0);
}

int main(int argc, char ** argv) {
	envrion = (char **) malloc(sizeof(char *) * 2);
	envrion[0] = "HOME=/home/testuser/haha";
	envrion[1] = "SHELL=/shell";
	envrion[2] = NULL;
	pthread_exec_count = 200;
	g_index = 0;

	pthread_t t_1;
	pthread_t t_2;
	pthread_t t_3;
	pthread_t t_4;

	pthread_create(&t_1, NULL, pthread_do3, NULL );
	pthread_create(&t_3, NULL, pthread_do4, NULL );
	pthread_create(&t_2, NULL, pthread_do3, NULL );
	pthread_create(&t_4, NULL, pthread_do4, NULL );

	pthread_join(t_1, NULL );
	pthread_join(t_2, NULL );
	pthread_join(t_3, NULL );
	pthread_join(t_4, NULL );

	exit(0);
}
