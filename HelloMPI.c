#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(int argc, char*argv[])
{
    char greeting[MAX_STRING];
    int comm_size;
    int my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
if(my_rank!=0)
{
sprintf(greeting, "Process %d of %d -> Have a Nice Day!", my_rank, comm_size);
  MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0,MPI_COMM_WORLD);
    } 
    else
     { 
          printf("Process %d of %d -> Have a Nice Day!\n", my_rank, comm_size);
            for (int q = 1; q < comm_size; q++)
             { 
                  MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
                  printf("%s\n", greeting); 
            } 
    } 
    MPI_Finalize(); 
    return 0;
     }
