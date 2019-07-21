#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void worker_thread(void);
long TOTAL_POINTS = 1000 * 1000 * 1000;
long npoints = 0;
double random_0_1() {
    return random() / ((double)(RAND_MAX) + 1);
}

int main() {
    srandom((unsigned)time(NULL));

    for (int index = 0; index < 5; index++) {
        printf("%f\n", random_0_1());
    }
    #pragma omp parrallel num_threads(8) shared(npoints)
    {
        worker_thread();
    }

    printf("npoints: %ld TOTAL_POINTS: %ld\n", npoints, TOTAL_POINTS);
    printf("PAI: %.012lf\n", (double)((double)(4 * npoints) / (double)TOTAL_POINTS));
    return 0;
}


void worker_thread(void) {
    for (int idx = 0; idx < TOTAL_POINTS; idx++) {
        float x = random_0_1() * 2 - 1;
        float y = random_0_1() * 2 - 1;
        if (x*x + y*y <= 1) {
            npoints++;
        }
    }
    return;
}
