#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>
int main(int argc,char*argv[])
{
   int my_rank,process_count;
   int*send_buf,*recv_buf;
   int recv_count;
   int k = (int)(strtol(argv[1],NULL,10));
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
   if(my_rank == 0)
   {
       send_buf = (int*)malloc(k * sizeof(int));
       for(int i=0; i<k ;i++)
       send_buf[i]=i+1;
       MPI_Send(send_buf,k,MPI_INT,1,0,MPI_COMM_WORLD);
       free(send_buf);
    }else {
        MPI_Status status;
        MPI_Probe(0,0,MPI_COMM_WORLD,&status);
        MPI_Get_count(&status,MPI_INT, &recv_count);
        recv_buf =(int*) malloc(recv_count* sizeof(int));
        MPI_Recv(recv_buf, recv_count, MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i=0; i<recv_count; i++)
        printf("index=%d, element value =%d\n",i,recv_buf[i]);
        free(recv_buf);
    }
MPI_Finalize();
}