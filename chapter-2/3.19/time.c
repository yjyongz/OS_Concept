#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	struct timeval start;
	struct timeval end;
	void *addr;
	int status;
	addr = mmap(NULL, sizeof(struct timeval) * 2, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		exit(0);
	}
	pid_t pid = fork();
	if (pid == 0) {
		gettimeofday(&start, NULL);
		//printf("child:%d second:%lu microsecond:%lu\n", getpid(), start.tv_sec, start.tv_usec);
		memcpy(addr, &start, sizeof(struct timeval));
		execvp(argv[1], &argv[1]);
	} else {
		pid_t child = waitpid(pid, &status, 0);
		//printf("child process id:%d %d\n", child, status);
		gettimeofday(&end, NULL);
		struct timeval tstart;
		memcpy(&tstart, addr, sizeof(struct timeval));
		printf("times taken for %s: second:%lu microsecond:%ld\n", argv[0], end.tv_sec - tstart.tv_sec, end.tv_usec - tstart.tv_usec);
		munmap(addr, sizeof(struct timeval));
	}
	return 0;
}
