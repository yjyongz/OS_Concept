#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>

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
}

void parse_sub_command(char **args[length], int index, int *out_to_file, 
                       int *in_to_file, int *eout_to_file, 
                       int **ifile, int **ofile, int **efile, 
                       int *out_combine, int *err_combine) {
    int t = 0, i_append = false, o_append = false;
    while (args[index][t] != NULL) {
        char *token = args[index][t];
        if (*in_to_file == true && *ifile == NULL) {
            *ifile = (int*)calloc(1, sizeof(int));
            **ifile = open(token, O_RDONLY);
            free(args[index][t]);
            args[index][t] = NULL;
        }
        if (*out_to_file == true && *ofile == NULL) {
            *ofile = (int*)calloc(1, sizeof(int));
            if (o_append) {
                **ofile = open(token, O_WRONLY | O_APPEND, 0666);
            } else {
                **ofile = open(token, O_CREAT | O_WRONLY, 0666);
            }
            o_append= false;
            free(args[index][t]);
            args[index][t] = NULL;
        }
        if (*eout_to_file == true && *efile == NULL) {
            *efile = (int*)calloc(1, sizeof(int));
            if (o_append) {
                **efile = open(token, O_WRONLY | O_APPEND, 0666);
            } else {
                **efile = open(token, O_CREAT | O_WRONLY, 0666);
            }
            o_append = false;
            free(args[index][t]);
            args[index][t] = NULL;
        }
        if (*token == '<' || *token == '>' || 
            (strlen(token) == 3 && token[0] == '2' && token[1] == '>' && token[2] == '\n')) {
            *in_to_file = *token == '<';
            *out_to_file = *token == '>';
            i_append = strlen(token) == 2 && token[1] == '<';
            o_append = strlen(token) == 2 && token[1] == '>';
            *eout_to_file = (strlen(token) == 2 && token[0] == '2' && token[1] == '>'); 
            free(args[index][t]);
            args[index][t] = NULL;
        }
        if (strlen(token) == 5 && strncmp(token, "2>&1\n", 5) == 0) {
            *out_combine = true;
            free(args[index][t]);
            args[index][t] = NULL;
        }
        if (strlen(token) == 5 && strncmp(token, "1>&2\n", 5) == 0) {
            *err_combine = true;
            free(args[index][t]);
            args[index][t] = NULL;
        }
        t++;
    }
}

void execute(int ncommands, char **args[length], int index, int *pfd, 
             int *ofile, int *ifile, int *efile) {
    if (ncommands == index) return;
    int fd[2], status = 0, childpid, file;
    int out_to_file = false, in_to_file = false, eout_to_file = false, out_combine = false, err_combine = false;
    if (pipe(fd) == -1) {
        exit(errno);
    }
    parse_sub_command(args, index, &out_to_file, &in_to_file, &eout_to_file, 
                      &ifile, &ofile, &efile, &out_combine, &err_combine);
    pid_t pid = fork();
    switch(pid) {
        case -1:
            break;
        case 0:
            if (ifile != NULL) {
                while((dup2(*ifile, STDIN_FILENO) == -1) && errno == EINTR) {}
            }
            if (pfd != NULL) {
                if (ifile == NULL) {
                    while((dup2(*ifile, STDIN_FILENO) == -1) && errno == EINTR) {}
                }
                close(pfd[0]);
                close(pfd[1]);
            }
            if (efile != NULL) {
                while((dup2(*efile, STDERR_FILENO) == -1) && errno == EINTR) {}
            } if (index + 1 != ncommands) {
                while((dup2(fd[1], STDERR_FILENO) == -1) && errno == EINTR) {}
            }
            if (ofile != NULL) {
                while((dup2(*ofile, STDOUT_FILENO) == -1) && errno == EINTR) {}
            } if (index + 1 != ncommands) {
                while((dup2(fd[1], STDOUT_FILENO) == -1) && errno == EINTR) {}
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
                free(ifile);
                ifile = NULL;
            }
            if (ofile) {
                close(*ofile);
                free(ofile);
                ofile = NULL;
            }
            execute(ncommands, args, index+1, fd, ofile, ifile, efile);
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
            if (getHistory(strs))
        }
        string_parse(strs, &ncommands, args);
        execute(ncommands, args, 0, NULL, NULL, NULL, NULL);
    }
    return 0;
}
