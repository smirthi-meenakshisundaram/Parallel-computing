#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define N 10000000
#define MAX_ERR 1e-6

__global__ void vector_add(float *out, float *a, float *b, int n)
 {
    int i=blockDim.x * blockId.x + threadId.x;
    
    // i is thread_id
   
    //if(i<numElements)
    if(i<n)
    {
        out[i] = a[i] + b[i];
         i += blockDim.x; 
    }
}

int main(){
    float *a, *b, *out; 
    float *dev_a, *dev_b, *dev_out;   // The arrays for the GPU device


    // Allocate memory 
     a = (float*)malloc( N * sizeof(float) );
    b = (float*)malloc( N * sizeof(float) );
    out = (float*)malloc( N * sizeof(float) );

    // Initialize array
    for(int i = 0; i < N; i++){
        a[i] = 1.0f;
        b[i] = 2.0f;
          
    }
    cudaMalloc( (void**)&dev_a, N * sizeof(float) );
     cudaMalloc( (void**)&dev_b, N * sizeof(float) );
     cudaMalloc( (void**)&dev_out, N * sizeof(float) );
      cudaMemcpy( dev_a, a, N * sizeof(float), cudaMemcpyHostToDevice );
     cudaMemcpy( dev_b, b, N * sizeof(float), cudaMemcpyHostToDevice );
// Main function
int threadsperblock = 256;
int blockspergrid = (N + threadsperblock - 1) /threadsperblock;

    vector_add <<<blockspergrid,threadsperblock>>> (dev_out, dev_a, dev_b, N);
    
    cudaMemcpy( out, dev_out, N * sizeof(float),cudaMemcpyDeviceToHost );
    // Verification
    for(int i = 0; i < N; i++){
        assert(fabs(out[i] - a[i] - b[i]) < MAX_ERR);
    }

    printf("out[0] = %f\n", out[0]);
    printf("PASSED\n");

    free(a);
    free(b);
    free(c);

    CudaFree (dev_a);
    CudaFree (dev_b);
    CudaFree (dev_c);
    return 0;
}