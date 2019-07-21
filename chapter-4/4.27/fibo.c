#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int sum; /* this data is shared by the thread(s) */
int *array = NULL;
void *runner(void *param); /* threads call this function */
int main(int argc, char *argv[]) 
{
    pthread_t tid;/* the thread identifier */
    pthread_attr_t attr; /* set of thread attributes */

    /* set the default attributes of the thread */
    pthread_attr_init(&attr);
    /* create the thread */

    pthread_create(&tid, &attr, runner, argv[1]);

    /* wait for the thread to exit */

    pthread_join(tid, NULL);
    for (int idx=0; idx < atoi(argv[1]); idx++) {
        printf("%d ",array[idx]);
    }
    printf("\n");

}


/* The thread will execute in this function */
void *runner(void *param)
{
    int i, length = atoi(param);
    array = calloc(1, sizeof(int) * length);
    sum = 0;
    array[1] = 1;
    for (i = 2; i < length; i++) {
        array[i] += array[i-1] + array[i-2];
    }
    pthread_exit(0);
}
