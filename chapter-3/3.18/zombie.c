#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>


int main() {
	pid_t pid = fork();
	int status = 0;
	if (pid == 0) {
		/*
		 * child proces
		 */
		//sleep(10);
		printf("child PID:%d\n", pid);
	} else {
		/*
		 * parent proces
		 */
		printf("parent PID:%d\n", pid);
		sleep(10);
		pid = wait(&status);
		printf("waited PID:%d\n", pid);
	}
	return 0;
}
