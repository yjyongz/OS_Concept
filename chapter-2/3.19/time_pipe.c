#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int parent_fd[2];
	int child_fd[2];
	int status;
	struct timeval start;
	struct timeval end;
	
	pid_t pid;	
	status = pipe(parent_fd);
	if (status == -1) {
		return 1;
	}
	status = pipe(child_fd);
	if (status == -1) {
		return 1;
	}

	pid = fork();
	if (pid == 0) {
		// child
		close(child_fd[0]);
		close(child_fd[1]);

		close(parent_fd[0]);
		gettimeofday(&start, NULL);
		write(parent_fd[1], &start, sizeof(struct timeval));
		execvp(argv[1], &argv[1]);
		close(parent_fd[1]);
	} else {
		// parent
		close(child_fd[0]);
		close(child_fd[1]);
		close(parent_fd[1]);
		pid_t child = waitpid(pid, &status, 0);
		gettimeofday(&end, NULL);
		read(parent_fd[0], &start, sizeof(struct timeval));
		printf(" time elapsed: %ld second %ld microsecond\n", end.tv_sec - start.tv_sec, end.tv_usec- start.tv_usec);
		close(parent_fd[0]);
	}
	return 0;
}
