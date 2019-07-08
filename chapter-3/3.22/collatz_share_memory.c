#include <stdio.h>
#include <stdint.h> // uint32_t
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid
#include <unistd.h> //  fork , pipe
#include <stdlib.h> // exit 
#include <fcntl.h> // O_ * constants
#include <sys/stat.h> // For mode constants
#include <sys/mman.h> // mmap , shm_open
#include <string.h> // memcpy

int main() {
	int s;
	pid_t pid;
	int size = 4096;
	void *addr = NULL;
	
	int fd = shm_open("ch_pa", O_RDWR| O_CREAT, S_IRUSR| S_IWUSR);
	// adjust the size
	ftruncate(fd, size);
	while (scanf("%d", &s) == 1) {
		addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		pid = fork();
		if (pid == 0) {
			// child
			printf("Child process %d %d \n", getpid(), s);
			int index = s;
			int bytes = 0;
			while (1) {
				memcpy(addr+bytes, &index, sizeof(uint32_t));
				bytes += sizeof(uint32_t);
				if (index == 1) break;
				if (index % 2 == 0) {
					index /= 2;
				} else {
					index = (index << 1) + index + 1;
				}
			}
			munmap(addr, size);
			exit(0);
		} else {
			int status;
			// parent
			pid_t childpid = waitpid(pid, &status, 0);
			printf("parent process %d child pid:%d %d status:%d\n", getpid(), childpid, s, status);
			int pbuffer[4096] = {0};
			memcpy(pbuffer, addr, size);
			int idx = 0;
			while (pbuffer[idx] != 0) {
				printf("index : %d ", pbuffer[idx]);
				idx++;
			}
			printf("\n");
			munmap(addr, size);
		}
	}
	printf("invalid input\n");
	return 0;
}
