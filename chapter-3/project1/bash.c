#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80 /* The maximum length command */
int should_run = 1;
void handler(int sig) {
	should_run = 0;	
}

const int length = MAX_LINE * 2;

void assign_args(char **args[length], int t, char buffer[64][64], int index) {
	args[index] = calloc(1, sizeof(char**)*(t+1)); 
	for (int idx=0; idx < t; idx++) {
		int len = strlen(buffer[idx]);
		args[index][idx] = calloc(1, sizeof(char) * len);
		strncpy(args[index][idx], buffer[idx], len);
	}
}

void string_parse(char *strs, int *ncommands, char **args[length])
{
	const char *DELIM = "\t\r\n\a ";
	int index = 0, t = 0;
	char buffer[64][64] = {0};
	char *token = strtok(strs, DELIM);
	while (token != NULL) {
		if (*token != '|') {
			memcpy(buffer[t], token, strlen(token));
			t+=1;
		} else {
			assign_args(args, t, buffer, index);
			index += 1;
			memset(buffer, 0, sizeof(buffer));
			t = 0;
		}
		token = strtok(NULL, DELIM);
	}

	assign_args(args, t, buffer, index);
	index+=1;
	*ncommands = index; 
	for (int idx = 0; idx < index; idx++) {
		for (int jdx=0; args[idx][jdx] != NULL; jdx++) {
			printf("%s ", args[idx][jdx]);
		}
		printf("\n");
	}
}

void execute(int ncommands, char **args[length], int index = 0) {

	if (ncommands == index) return;

	pid_t pid = fork();
	switch(pid) {
		case 0:
			//child
			break
		case -1:
			break;
		default:
			int childpid = waitpid(pid, status, 0);
			//parent
	}
}
int main(void)
{
	signal(SIGINT, handler);
	char **args[length];
	/* e.g: ls -l | less
     * args[0][0] = ls
	 * args[0][1] = -l
	 * args[0][2] = NULL
     *
     * args[1][0] = less
     * args[1][1] = NULL
	 */
	while (should_run) {
		char strs[MAX_LINE] = {0};
		memset(args, 0, sizeof(args));
		printf("osh#");
		fgets(strs, length, stdin);
		int ncommands = 0;
		string_parse(strs, &ncommands, args);
		execute(ncommands, args);
	}
	return 0;
}
