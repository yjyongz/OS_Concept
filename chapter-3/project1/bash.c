#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

#define MAX_LINE 80 /* The maximum length command */
int should_run = 1;
void handler(int sig) {
    should_run = 0;	
}
const int length = MAX_LINE * 2;

void assign_args(char **args[length], int t, char buffer[64][64], int index) {
    args[index] = calloc(1, sizeof(char**)*(t+1)); 
    int idx = 0;
    for (idx=0; idx < t; idx++) {
        int len = strlen(buffer[idx]);
        args[index][idx] = calloc(1, sizeof(char) * len);
        strncpy(args[index][idx], buffer[idx], len);
    }
}

void string_parse(char *strs, int *ncommands, char **args[length])
{
    const char *DELIM = "\t\r\n\a ";
    int index = 0, t = 0, idx = 0, jdx = 0;
    char buffer[64][64] = {{0}};
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
    for (idx = 0; idx < index; idx++) {
        for (jdx=0; args[idx][jdx] != NULL; jdx++) {
            printf("%s ", args[idx][jdx]);
        }
        printf("\n");
    }
}

void execute(int ncommands, char **args[length], int index, int *pfd, 
             int *ofile, int *ifile) {

    if (ncommands == index) return;
    int fd[2], status = 0, childpid, file;
    int out_to_file = false, in_to_file = false;
    if (pipe(fd) == -1) {
        exit(errno);
    }
    if (index + 2 < ncommands) {
        out_to_file = args[index+1][0][0] == '>';
        in_to_file = args[index+1][0][0] == '<';
    }
    if (in_to_file && ifile == NULL) {
        *ifile = open(args[index+2][0], O_CREAT | O_WRONLY);
    }
    if (out_to_file && ofile == NULL) {
        *ofile = open(args[index+2][0], O_CREAT | O_WRONLY);
    }
    pid_t pid = fork();
    switch(pid) {
        case -1:
            break;
        case 0:
            if (ifile != NULL) {
                dup2(*ifile, STDIN_FILENO);
            }
            if (pfd != NULL) {
                if (ifile == NULL) {
                    dup2(pfd[0], STDIN_FILENO);
                }
                close(pfd[0]);
                close(pfd[1]);
            }
            if (ofile != NULL) {
                dup2(*ofile, STDOUT_FILENO);
            } else {
                dup2(fd[1], STDOUT_FILENO);
            }
            close(fd[0]);
            close(fd[1]);
            execvp(args[index][0], args[index]);
        default:
            if (pfd) {
                close(pfd[0]);
                close(pfd[1]);
            }
            childpid = waitpid(pid, &status, 0);
            if (ifile) {
                close(*ifile);
                ifile = NULL;
            }
            if (ofile) {
                close(*ofile);
                ofile = NULL;
            }
            execute(ncommands, args, index+1, fd, ofile, ifile);
            close(fd[0]);
            close(fd[1]);
    }
}

int main(void)
{
    signal(SIGINT, handler);
    /* 
     * e.g: ls -l | less
     * args[0][0] = ls
     * args[0][1] = -l
     * args[0][2] = NULL
     *
     * args[1][0] = less
     * args[1][1] = NULL
     */
    char **args[length];
    while (should_run) {
        int ncommands = 0;
        char strs[MAX_LINE] = {0};
        memset(args, 0, sizeof(args));
        printf("osh#");
        fgets(strs, length, stdin);
        if (strs[0] == '\n') continue;
        if (strlen(strs) == 3 && 
            strs[0] == '!' && strs[1] == '!' && strs[2] == '\n') {
            printf("osh#");
            //getHistory(strs);
        }
        string_parse(strs, &ncommands, args);
        execute(ncommands, args, 0, NULL, NULL, NULL);
    }
    return 0;
}
