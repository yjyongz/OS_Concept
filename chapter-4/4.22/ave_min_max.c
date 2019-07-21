#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define LENGTH 7

float ave = 0;
int mmin;
int mmax;
void* func_average(void *array) {
    int *t = (int*)array;
    for (int index = 0; index < LENGTH; index++) {
        ave += t[index];
    }
    ave = ave/LENGTH;
    return NULL;
}

void* func_min(void *array) {
    int *t = (int*)array;
    sleep(1);
    mmin = t[0];
    for (int index = 0; index < LENGTH; index++) {
        mmin = fmin(mmin, t[index]);
    }
    return NULL;
}

void* func_max(void *array) {
    int *t = (int*)array;
    sleep(3);
    mmax = t[0];
    for (int index = 0; index < LENGTH; index++) {
        mmax = fmax(mmax, t[index]);
    }
    return NULL;
}

int main() {
    pthread_t thread[3];
    int array[LENGTH] = {90,81,78,95,79,72,85};
    int ret;
    void *res;
    void *func[3] = {func_average, func_min, func_max};
    memset(&thread, 0, 3);

    for (int index = 0; index < 3; index++) {
        ret = pthread_create(&thread[index], NULL, 
                             func[index], (void*)array);
        if (ret != 0) {
            perror(strerror(ret));
        }
    }
    for (int index = 0; index < 3; index++) {
        ret = pthread_join(thread[index], &res);
        if (ret != 0) {
            perror(strerror(ret));
        }
        printf("Join with index: %d return value %s %f %d %d\n", 
                index, (char*)res, ave, mmin, mmax);
    }
}
