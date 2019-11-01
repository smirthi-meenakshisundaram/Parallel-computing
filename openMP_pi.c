// To compile the code: gcc -g -Wall -fopenmp -o pi openMP_pi.c
// To run the code : ./pi 4
// Input : The number of threads should be given

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static long iterations = 10000;
double s;

int main (int argc, const char *argv[]) {
    int thread_count;
    int i,j;
    double x;
    double pi, sum = 0.0;
     
     thread_count = strtol(argv[1],NULL,10);

    s = 1.0/(double) iterations;
    
    for (j=1; j<= thread_count; j++) {

        printf(" The output running on %d threads: ", j);
    
     omp_set_num_threads(j);
     sum = 0.0;
         
        #pragma omp parallel for reduction(+:sum) private(x)
        for (i=0; i < iterations; i++) {
            x = (i+0.5)*s;
            sum += 4.0 / (1.0+x*x); 
        }
        pi = s * sum;
        printf("pi = %.16g \n", pi); 
    }
    }