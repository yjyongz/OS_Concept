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
	char *file = "/tmp/test-1"; 
	mkfifo(file, 0666);
	char s[256] = {0};
	signal(SIGINT, handle);
	while (1) {
		int fd1 = open(file, O_RDONLY);
		read(fd1, s, 256);
		char t[256] ={0};
		for (int index= 0 ; index < strlen(s); index++) {
			t[index] = s[strlen(s) - index -1];
		}
		close(fd1);

		int fd = open(file, O_WRONLY);
		write(fd, t, 256);
		close(fd);
	}
	return 0;
}
