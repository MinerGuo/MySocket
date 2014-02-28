#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>

int main(int argc, char ** argv) {
	pid_t pid;
	struct rlimit rl;
	int fd0, fd1, fd2,i;
	struct sigaction sa;

	//step1:将文件屏蔽字设置为0，这样就可以将可能继承来的文件创建屏蔽字给清理掉
	umask(0);

	//step2:调用fork，然后将父进程退出
	//好处是，第一，如果该守护进程是由shell命令启动，那么父进程的终止，会使shell认为这条命令已经执行完毕
	//第二：子进程继承了父进程的进程组ID，但具有一个新的进程ID，这就保证了子进程不是一个进程组的组长进程，
	//这就为下面的setsid提供了前提条件
	pid = fork();
	if (pid > 0) {
		//父进程退出
		exit(0);
	}

	//这里开始是子进程了

	//step3:setsid
	//创建新会话，使得当前进程为新会话的首进程，并成为组长进程，并没有控制终端
	setsid();

	//忽略因为终端断开而产生的信号
	sa.sa_handler = SIG_IGN;	//处理方式为忽略
	sigemptyset(&sa.sa_mask);	//信号处理期间不阻塞任何其他信号
	sa.sa_flags = 0;

	sigaction(SIGHUP, &sa, NULL ); //SIGHUP即为 终端断开产生的信号
	//再次使父进程退出
	pid = fork();
	if (pid > 0) {
		//父进程退出
		exit(0);
	}

	//step4:将当前工作目录设置为根目录，防止当前工作目录位于装配文件中
	chdir("/");

	//step5:关闭不再需要的文件描述符
	getrlimit(RLIMIT_NOFILE, &rl);
	if (rl.rlim_max == RLIM_INFINITY ) {
		rl.rlim_max = 1024;
	}
	for (i = 0; i < rl.rlim_max; i++) {
		close(i);
	}

	//step6:打開/dev/null，使其具有文件描述符0、1、2,从而关闭标准输入、输出、错误输出
	fd0 = open("dev/null", O_RDWR);
	fd1 = dup(fd0);
	fd2 = dup(fd0);

	//主要是想让它一直运行下去，有利于测试
	for(;;){

	}

	exit(0);
}
