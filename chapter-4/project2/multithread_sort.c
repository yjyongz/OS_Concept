#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


struct range {
    int start;
    int end;
};

int array[10] = {7,12,19,3,18,4,2,6,15,8};

void merge(int start, int mid, int end) {
    int index = start;
    int s1,s2;
    s1 = start;
    s2 = mid + 1;
    int *tarr = (int*)calloc(1, sizeof(int) * ((end - start) + 1));
    while (s1 <= mid && s2 <= end) {
        if (array[s1] <= array[s2]) {
            tarr[index++] = array[s1++];
        } else {
            tarr[index++] = array[s2++];
        }
    }

    while (s1 <= mid) {
        tarr[index++] = array[s1++];
    }
    while (s2 <= end) {
        tarr[index++] = array[s2++];
    }

    for (int idx = start; idx <= end; idx++) {
        array[idx] = tarr[idx];
    }
    free(tarr);
}

void mergeSort(int start, int end) {
    if (start == end) return;
    int mid = (start+end) / 2;
    mergeSort(start, mid);
    mergeSort(mid+1, end);
    merge(start, mid, end);

}

void* start_section(void* param) {
    struct range rg = *(struct range*)param;
    mergeSort(rg.start, rg.end);
    return NULL;
}

int main() {
    pthread_t first;
    pthread_t second;
    struct range rg;
    rg.start = 0;
    rg.end = 4;
    for (int idx = 0; idx < 10; idx++) {
        printf("%d ", array[idx]);
    }
    printf("\n");
    pthread_create(&first, NULL, start_section, &rg);
    struct range rg1;
    rg1.start = 5;
    rg1.end = 9;
    pthread_create(&second, NULL, start_section, &rg1);

    pthread_join(first, NULL);
    pthread_join(second, NULL);
    merge(0, 4, 9);

    for (int idx = 0; idx < 10; idx++) {
        printf("%d ", array[idx]);
    }
    printf("\n");
    return 0;
}
