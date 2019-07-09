#include <stdio.h>
#include <sys/stat.h> // mode_t
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

char *file = "/tmp/test-1"; 
void handle(int sig) {
	printf("Handled Signal %d\n", sig);
	unlink(file);
	exit(0);
}

int main() {
	mkfifo(file, 0666);
	char s[256] ={0};
	signal(SIGINT, handle);
	while (fgets(s, 256, stdin) != NULL) {
		int fd = open(file, O_WRONLY);
		write(fd, s, strlen(s));
		close(fd);
		int fd1 = open(file, O_RDONLY);
		read(fd1, s, 256);
		close(fd1);
		printf("received: %s\n", s);
	}
	unlink(file);
	return 0;
}
