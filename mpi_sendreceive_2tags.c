//Two sends and receives, The receive will get data from the send with the same tag
//To compile  mpicc -g -Wall -o sr2 mpi_sendreceive_2tags.c
// To execute  mpiexec -n 2 sr2
// OUTPUT Received y from MPI_send with tag 1 is : 90
//Received y from MPI_send with tag 1 is : 100

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>

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
        x+=10;
         MPI_Send(&x,1,MPI_INT,1,2,MPI_COMM_WORLD);
    }
    else
    {
        int y;
        MPI_Recv(&y,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
        printf("Received y from MPI_send with tag 1 is : %d \n",y);

         MPI_Recv(&y,1,MPI_INT,0,2,MPI_COMM_WORLD,&status);
         printf("Received y from MPI_send with tag 1 is : %d \n",y);
    }
    MPI_Finalize();
    return 0;
}