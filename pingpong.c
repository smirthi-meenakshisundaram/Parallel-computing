#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>

const int MAX_STRING = 100;
int main(int argc, char *argv[])
{

    int count = 0;
    int rank, process_count;

    if (MPI_Init(&argc, &argv))
    {
        printf("Error MPI_Init\n");
        exit(-1);
    }

    if (MPI_Comm_size(MPI_COMM_WORLD, &process_count))
    {
        printf("Error MPI_Comm_Size\n");
        exit(-1);
    }

    if (MPI_Comm_rank(MPI_COMM_WORLD, &rank))
    {
        printf("Error MPI_Comm_rank\n");
        exit(-1);
    }
    MPI_Status status;
    int partner = rank%2==0?rank+1:rank-1;
    while (count < 10)
    {
        if ((rank % 2) == (count%2==0))
        {
            count++;
            printf("The Player: %d send the count: %d from player: %d\n",rank, count,partner);
            MPI_Send(&count, 1, MPI_INT, partner, 1, MPI_COMM_WORLD);
            MPI_Recv(&count, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
            //printf("%d partner 1 \n",partner)
        }
        else
        {
            MPI_Recv(&count, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &status);
            count++;
            printf("The Player: %d received the count: %d from player:%d\n",rank, count,partner);
            //printf("%d partner2 \n",partner);
            MPI_Send(&count, 1, MPI_INT, partner, 1, MPI_COMM_WORLD);
        }
    }
    if ((rank % 2) != (count%2==0))
        printf("Game finishes with Player %d with count %d\n",rank+1, count);
    
    fflush(stdout);
    MPI_Finalize();
    return 0;
}