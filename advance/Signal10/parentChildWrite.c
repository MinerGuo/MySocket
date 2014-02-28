/*
 * parentChildWrite.c
 *
 *  Created on: 2013-5-26
 *      Author: miner
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

int write_flag = 0;
static int num = 1;
sigset_t t_sigset_usr, t_sigset_old, t_sigset_zero;

void write_num_to_file();

void tell_parent();
void tell_child(pid_t child_p_id);
void wait_parent();
void wait_child();
void do_sig_usr(int signo);

//父进程将只接受SIGUSR1信号
void tell_parent() {
	kill(getppid(), SIGUSR1);
}

//子进程将只接受SIGUSR2信号
void tell_child(pid_t child_p_id) {
	kill(child_p_id, SIGUSR2);
}

/**
 * 与wait_child相同
 */
void wait_parent() {
	//判断其是否等于0 ，原因是任何信号都会使sigsuspend执行完毕，而在usr的信号处理中，将其设置为1
	//这样，就表示除非usr信号发送，否则永远在这里等待下去
	while (write_flag == 0) {
		//除了空信号，允许所有信号均传递到进程
		sigsuspend(&t_sigset_zero);
	}
	//usr信号确实发生了，再将标志为置回去
	write_flag = 0;
	//恢复原来的所有信号集，也就是不再接受usr信号
	sigprocmask(SIG_SETMASK, &t_sigset_old, NULL );
}
void wait_child() {
	//判断其是否等于0 ，原因是任何信号都会使sigsuspend执行完毕，而在usr的信号处理中，将其设置为1
	//这样，就表示除非usr信号发送，否则永远在这里等待下去
	while (write_flag == 0) {
		//除了空信号，允许所有信号均传递到进程
		sigsuspend(&t_sigset_zero);
	}
	//usr信号确实发生了，再将标志为置回去
	write_flag = 0;
	//恢复原来的所有信号集，也就是不再接受usr信号
	sigprocmask(SIG_SETMASK, &t_sigset_old, NULL );
}

void do_sig_usr(int signo) {
	write_flag = 1;
}

void write_num_to_file() {

	printf("%d:%d\n", getpid(), num);
	fflush(NULL);
	num++;
	num++;
	/*
	 char num_buff[128];
	 memset(num_buff, '\0', sizeof(char) * 128);
	 sprintf("%d:%d\n", getpid(), num);
	 write(fd, num_buff, sizeof(char) * 128);*/
}

int main(int argc, char ** argv) {

	//初始化两个信号集，一个只有usr1，一个只有usr2
	sigemptyset(&t_sigset_usr);
	sigemptyset(&t_sigset_old);
	sigemptyset(&t_sigset_zero);

	sigaddset(&t_sigset_usr, SIGUSR1);
	sigaddset(&t_sigset_usr, SIGUSR2);

	//注册信号处理程序，
	signal(SIGUSR1, do_sig_usr);
	signal(SIGUSR2, do_sig_usr);

	//父进程阻塞usr信号
	sigprocmask(SIG_SETMASK, &t_sigset_usr, NULL );

	pid_t child_pid;

	//父进程fork一个子进程出来
	child_pid = fork();
	if (child_pid == 0) {
		//子进程,从2开始写
		num = 2;

		//注册信号处理程序，
		signal(SIGUSR1, do_sig_usr);
		signal(SIGUSR2, do_sig_usr);

		//子进程阻塞usr信号
		sigprocmask(SIG_SETMASK, &t_sigset_usr, NULL );

		//子进程准备完毕，通知父进程去写
		tell_parent();

		while (1) {
			//始终等待父进程唤醒
			wait_parent();
			write_num_to_file();
			tell_parent();
		}

	} else {
		//父进程
		while (1) {
			//始终等待子进程唤醒
			wait_child();
			write_num_to_file();
			tell_child(child_pid);
		}

	}

	//子进程写入数据，并且告知父进程，然后投入睡眠，等待父进程唤醒

	//父进程等待子进程

	exit(0);
}
