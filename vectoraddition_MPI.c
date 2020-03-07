#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "mpi.h"

#define N 9
#define MAX_ERR 1e-6

 

int main(int argc,char*argv []){
    float *a, *b, *out; 
    int total_proc;	 // total nuber of processes	
	int rank;        // rank of each process
	int n_per_proc;	// elements per process	
    int i;
	
		
	// 1. Initialization of MPI environment
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &total_proc);
	// 2. Now you know the total number of processes running in parallel
	MPI_Comm_rank (MPI_COMM_WORLD,&rank);
	// 3. Now you know the rank of the current process
	
	// Smaller arrays that will be held on each separate process
    	float * aa;
	    float * bb;
	    float * cc;
	

    // Allocate memory
    if (rank == 0)
    {
    a   = (float*)malloc(sizeof(float) * N);
    b   = (float*)malloc(sizeof(float) * N);
    out = (float*)malloc(sizeof(float) * N);

    // Initialize array
    for(int i = 0; i < N; i++){
        a[i] = 1.0f;
        b[i] = 2.0f;
    }
    }
    n_per_proc = N/total_proc;

    aa = (float *) malloc(sizeof(float)*n_per_proc);
	bb = (float *) malloc(sizeof(float)*n_per_proc);
	cc = (float *) malloc(sizeof(float)*n_per_proc);

    MPI_Scatter(a, n_per_proc, MPI_FLOAT, aa, n_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD); 
	 
	MPI_Scatter(b, n_per_proc, MPI_FLOAT, bb, n_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD); 

    for(i=0;i<n_per_proc;i++)
		cc[i] = aa[i]+bb[i];

        MPI_Gather(cc, n_per_proc, MPI_FLOAT,out, n_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if (rank == 0)  {			
		// sanity check the result  (a test we would eventually leave out)
		int true = 1;
		for(i=0;i<N;i++) {
			
			if (out[i] != a[i] + b[i]) {
				printf("problem at index");
				true = 0;
				break;
			}
		}
		if (true) {
			  printf("out[0] = %f\n", out[0]);
              printf("PASSED\n");

		}
		
	}

	// clean up memory
	if (rank == 0)  {
		free(a);  
        free(b); 
        free(out);
	}
	free(aa); 
     free(bb);
      free(cc);
	
	// 9. Terminate MPI Environment and Processes
	MPI_Finalize();  
	
	return 0;
}

   

    
