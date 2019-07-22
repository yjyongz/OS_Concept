#include <stdio.h>
#include <pthread.h>


struct range {
    int start;
    int end;
};

int array[10] = {7,12,19,3,18,4,2,6,15,8};

void merge(int start, int mid, int end) {
    int index = 0;
    int s1,s2;
    s1 = start;
    s2 = mid + 1;
    while (s1 != mid + 1 && s2 + 1 != end) {
        if (array[s1] <= array[s2]) {
            array[index++] = array[s1++];
        } else {
            array[index++] = array[s2++];
        }
    }

    while (s1 != mid + 1) {
        array[index++] = array[s1++];
    }
    while (s2 + 1 != end) {
        array[index++] = array[s1++];
    }
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
    rg.end = 5;
    pthread_create(&first, NULL, start_section, &rg);
    struct range rg1;
    rg.start = 6;
    rg.end = 10;
    pthread_create(&second, NULL, start_section, &rg);

    pthread_join(first, NULL);
    pthread_join(second, NULL);

    for (int idx = 0; idx < 10; idx++) {
        printf("%d ", array[idx]);
    }
    printf("\n");
    return 0;
}
