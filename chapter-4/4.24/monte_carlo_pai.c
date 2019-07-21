#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t lock;
void* worker_thread(void* args);
int TOTAL_POINTS = 1000*1000*10;
long npoints = 0;
long outpoints = 0;
double random_0_1() {
    return random() / ((double)(RAND_MAX) + 1);
}

int main() {
    pthread_t thread_id[5];
    void *res = NULL;
    srandom((unsigned)time(NULL));
    pthread_mutex_init(&lock, NULL);

    for (int index = 0; index < 5; index++) {
        printf("%f\n", random_0_1());
        pthread_create(&thread_id[index], NULL, worker_thread, NULL);
    }
    for (int index = 0; index < 5; index++) {
        pthread_join(thread_id[index], res);
    }

    printf("npoints: %ld TOTAL_POINTS: %ld\n", npoints, outpoints);
    printf("PAI: %.012lf\n", (double)((double)(4 * npoints) / (double)outpoints));
    return 0;
}


void* worker_thread(void* args) {
    for (int idx = 0; idx < TOTAL_POINTS; idx++) {
        float x = random_0_1() * 2 - 1;
        float y = random_0_1() * 2 - 1;
        if (x*x + y*y <= 1) {
            pthread_mutex_lock(&lock);
            npoints++;
            pthread_mutex_unlock(&lock);
        }
        pthread_mutex_lock(&lock);
        outpoints++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
