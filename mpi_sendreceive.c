// To compile  mpicc -g -Wall -o sendr mpi_sendreceive.c
// To execute mpiexec -n 2 sendr
// Since we use 2 processes we give 2 after -n as a command line arguement for executing

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>
int main(int argc,char*argv[])
{
MPI_Init(NULL,NULL);
int rank;
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Status status;

if(rank==0)
{
    int x=90;
    MPI_Send(&x,1,MPI_INT,1,1,MPI_COMM_WORLD);
}
else
{
    int y;
    MPI_Recv(&y,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
    printf("The value received from x using send receive which is y is : %d",y);
}
MPI_Finalize();
return 0;
}
