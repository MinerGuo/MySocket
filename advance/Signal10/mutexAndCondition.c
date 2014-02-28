/*
 * mutexAndCondition.c
 *
 *  Created on: 2013-6-1
 *      Author: miner
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//互斥变量，用于控制在读写队列时的加锁
pthread_mutex_t g_qlock = PTHREAD_MUTEX_INITIALIZER;
//条件变量，用于唤醒处于等待队列的线程
pthread_cond_t g_qready = PTHREAD_COND_INITIALIZER;

int id = 0;

struct message {
	int m_msg_id;
};

struct message_queue {
	struct message * p_next;
};

struct message_queue workq;

void deal_msg(struct message * p_msg) {
	printf("msg id is %d\n", p_msg->m_msg_id);
	fflush(NULL );

}

void * process_msg(void * arg) {

	for (;;) {
		//先获取互斥量
		pthread_mutex_lock(&g_qlock);
		while (workq.p_next == NULL ) {
			//等待条件变量被设置，当然也需要对互斥量进行解锁，因为写线程也需要对条件变量加锁后才能写入
			//这两个操作，显然，需要原子性操作去完成，pthread_cond_wait便是
			pthread_cond_wait(&g_qready, &g_qlock);			//这里返回后，会自动对互斥量进行加锁

		}
		deal_msg(workq.p_next);
		free(workq.p_next);
		workq.p_next = NULL;
		//解锁互斥量
		pthread_mutex_unlock(&g_qlock);
	}

	return 0;
}

void * add_msg_to_quence(void * arg) {
	for (;;) {
		//先获取互斥量
		pthread_mutex_lock(&g_qlock);
		struct message * p_mess = (struct message *) malloc(sizeof(struct message));
		p_mess->m_msg_id = id;
		workq.p_next = p_mess;
		id++;

//解锁
		pthread_mutex_unlock(&g_qlock);
		//设置条件变量为真
		pthread_cond_signal(&g_qready);
	}
	return 0;
}

/**
 * 线程间通信，一个线程用于产生队列，另一个线程用于消费队列
 */
int main(int argc, char ** argv) {
	pthread_t t_produce;
	pthread_t t_customer;

	//创建消费者线程
	pthread_create(&t_customer, NULL, process_msg, NULL );
	//创建生产者线程
	pthread_create(&t_produce, NULL, add_msg_to_quence, NULL );

	pthread_join(t_customer, NULL );
	pthread_join(t_produce, NULL );

}
