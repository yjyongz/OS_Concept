#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define BUFFERSIZE 256

int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		printf("file not found\n");
		exit(0);
	}
	struct stat 	st;
	char 			buffer[BUFFERSIZE] = {0};
	int 			fds[2];
	int 			status;
	int 			childpid;
	int 			size = 0;
	int 			length = 0;

	stat(argv[1], &st);
	if (pipe(fds) == -1) {
		exit(0);
	}
	pid_t pid = fork();
	if (pid == 0) {
		fd = open(argv[2], O_WRONLY | O_CREAT, 0666);
		while ((size = read(fds[0], buffer, BUFFERSIZE)) != 0) {
			write(fd, buffer, size);
			length += size;
			if (length == st.st_size) break;
		}
		close(fd);
		close(fds[1]);
		close(fds[0]);
	} else {
		while ((size = read(fd, buffer, BUFFERSIZE)) != 0) {
			write(fds[1], buffer, size);
		}
		childpid = waitpid(pid, &status, 0);
		close(fd);
		close(fds[1]);
		close(fds[0]);
	}
	return 0;
}
