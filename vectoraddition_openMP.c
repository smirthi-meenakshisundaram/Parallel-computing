#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <omp.h>
void vector_add(float *out, float *a, float *b, int n,int thread_count);

#define N 10000000
#define MAX_ERR 1e-6



    void vector_add(float *out, float *a, float *b, int n,int thread_count) {
    #pragma omp parallel for num_threads(thread_count)
    for(int i = 0; i < n; i++)
    {
        out[i] = a[i] + b[i];
    }
    }

int main()
{
     float *a, *b, *out;
    int thread_count;

        printf("\n Enter the thread count:");
        scanf("%d",&thread_count);
// Allocate memory
     a   = (float*)malloc(sizeof(float) * N);
     b   = (float*)malloc(sizeof(float) * N);
     out = (float*)malloc(sizeof(float) * N);

    // Initialize array
    for(int i = 0; i < N; i++){
        a[i] = 1.0f;
        b[i] = 2.0f;
    }
    

    // Main function
    vector_add(out, a, b, N,thread_count);

    // Verification
   #pragma omp parallel for num_threads(thread_count)
    for(int i = 0; i < N; i++){
        assert(fabs(out[i] - a[i] - b[i]) < MAX_ERR);
    }

    printf("out[0] = %f\n", out[0]);
    printf("PASSED\n");
}

    
