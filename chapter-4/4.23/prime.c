#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

void* func(void *n) {
    int index, jdx;
    int size = *(int*)n;
    bool flag = true;
    index = 0;
    jdx = 0;
    for (jdx = 1; jdx <=size; jdx++) {
        flag = true;
        for (index = 2; index <= (int)(sqrt(jdx)+1); index++) {
            if (jdx % index == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            printf("%d ", jdx);
        }
    }
    printf("\n");
    return NULL;
}

int main() {
    int n = 0;
    pthread_t thread;
    void *res;
    scanf("%d", &n);
    pthread_create(&thread, NULL, func, &n); 
    pthread_join(thread, &res);
    return 0;
}
