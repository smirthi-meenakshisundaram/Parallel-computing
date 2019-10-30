

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include "my_rand.h"

const int MAX_NUMBERS = 100000;
int main(int argc, char *argv[])
{

    int sum = 0, recv_number = 0;
    int my_rank, process_count;

    if (MPI_Init(&argc, &argv))
    {
        printf("Error in getting MPI_Init\n");
        exit(-1);
    }

    if (MPI_Comm_size(MPI_COMM_WORLD, &process_count))
    {
        printf("Error in getting MPI_Comm_Size\n");
        exit(-1);
    }

    if (MPI_Comm_rank(MPI_COMM_WORLD, &my_rank))
    {
        printf("Error in getting MPI_Comm_rank\n");
        exit(-1);
    }
    unsigned seed = my_rank, x;
    int next_rank = (my_rank + 1) % process_count;
    int pre_rank = my_rank == 0 ? process_count - 1 : my_rank - 1;
    x = my_rand(&seed);
    x = my_rand(&x);
    int rand_num = my_drand(&x) * MAX_NUMBERS;
    if (my_rank == 0)
    {
        MPI_Send(&rand_num, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_number, 1, MPI_INT, pre_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d recieved the sum  %d from  Process %d\n", my_rank, recv_number, pre_rank);
    }
    else
    {
        MPI_Recv(&recv_number, 1, MPI_INT, pre_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum = rand_num + recv_number;
        printf("Process %d generated a random number %d and calculated the sum to be %d\n", my_rank, rand_num, sum);
        MPI_Send(&sum, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }
    fflush(stdout);
    MPI_Finalize();
    return 0;
}