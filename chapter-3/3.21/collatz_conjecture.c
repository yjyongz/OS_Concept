#include <stdio.h>
#include <sys/types.h> // pid_t
#include <sys/wait.h> // pid_t
#include <ctype.h> //  isdigit
#include <unistd.h> //  fork , pipe
#include <stdlib.h> // exit 

int main() {
	int s;
	int cfd[2];
	int pfd[2];
	pid_t pid;

	while (scanf("%d", &s) == 1) {
		if (pipe(cfd) == -1) exit(1);
		if (pipe(pfd) == -1) exit(1);

		pid = fork();
		if (pid == 0) {
			// child
			close(cfd[0]);
			close(cfd[1]);
			close(pfd[0]);
			printf("Child process %d %d pfd[1] = %d\n", getpid(), s, pfd[1]);
			int index = s;
			while (1) {
				int bytes = write(pfd[1], &index, sizeof(int));
				if (index == 1) break;
				if (index % 2 == 0) {
					index /= 2;
				} else {
					index = (index << 1) + index + 1;
				}
				printf("index = %d\n", index);
			}
			close(pfd[1]);
			exit(0);
		} else {
			int status;
			// parent
			pid_t childpid = waitpid(pid, &status, 0);
			printf("parent process %d child pid:%d %d status:%d\n", getpid(), childpid, s, status);
			close(pfd[1]);
			close(cfd[0]);
			close(cfd[1]);
			int index;
			while (read(pfd[0], &index, sizeof(int)) != 0) {
				printf("%d ", index);
			}
			printf("\n");
			close(pfd[0]);
		}
	}
	printf("invalid input\n");
}
