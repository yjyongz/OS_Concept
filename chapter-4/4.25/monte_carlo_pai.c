#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void worker_thread(void);
long TOTAL_POINTS = 1000 * 1000 * 1000;
long npoints = 0;
long totalpoints = 0;
double random_0_1() {
    return random() / ((double)(RAND_MAX) + 1);
}

int main() {
    srandom((unsigned)time(NULL));

    for (int index = 0; index < 5; index++) {
        printf("%f\n", random_0_1());
    }
    #pragma omp parrallel num_threads(8) shared(npoints,totalpoints)
    {
        worker_thread();
    }

    printf("npoints: %ld TOTAL_POINTS: %ld\n", npoints, totalpoints);
    printf("PAI: %.012lf\n", (double)((double)(4 * npoints) / (double)totalpoints));
    return 0;
}


void worker_thread(void) {
    for (int idx = 0; idx < TOTAL_POINTS; idx++) {
        float x = random_0_1() * 2 - 1;
        float y = random_0_1() * 2 - 1;
        if (x*x + y*y <= 1) {
            npoints++;
        }
        totalpoints++;
    }
    return;
}
