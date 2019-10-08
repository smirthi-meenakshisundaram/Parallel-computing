#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>

const int MAX_STRING = 100;
int main(int argc, char *argv[])
{

    char *message = malloc(MAX_STRING * sizeof(char));

    int my_rank, process_count;

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

    if (MPI_Comm_rank(MPI_COMM_WORLD, &my_rank))
    {
        printf("Error MPI_Comm_rank\n");
        exit(-1);
    }
    MPI_Status status;
    // 2.a. master process receive the message from slave process 1 to $process_count - 1, one by one. If the message from the lower-rank process is not received, the master process has to wait.
    // Begining
    if (my_rank != 0)
    {
        sprintf(message, "First-round-process %d of %d> Have a nice day!", my_rank, process_count);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    }
    else
    {
        for (int i = 1; i < process_count; i++)
        {
            MPI_Recv(message, MAX_STRING, MPI_CHAR, i, 1, MPI_COMM_WORLD, &status);

            printf("First round: %d receive a message from %d with tag %d: %s\n", my_rank, status.MPI_SOURCE, status.MPI_TAG, message);
            fflush(stdout);
        }
        printf("Message received rank wise\n");
    }

    printf("Process %d of %d> DONE in the first round!\n", my_rank, process_count);
    // Ending of 2.a.

    // 2.b. master process could receive the message from slave process in any order
    // Starting
    if (my_rank != 0)
    {
        sprintf(message, "Second-round-process %d of %d> Have a nice day!", my_rank, process_count);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (int i = 1; i < process_count; i++)
        {
            MPI_Recv(message, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            printf("Second round: %d receive a message from %d with tag %d: %s\n", my_rank, status.MPI_SOURCE, status.MPI_TAG, message);
            fflush(stdout);
        }
        printf("Message received from any source\n");
    }

    printf("Process %d of %d> DONE in the second round!\n", my_rank, process_count);
    // Ending of 2.b

    // 2.c. master process could receive the message from slave process based on any positive tags
    // Starting

    if (my_rank != 0)
    {
        sprintf(message, "Third-round-process %d of %d> Have a nice day!", my_rank, process_count);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, my_rank, MPI_COMM_WORLD);
    }
    else
    {
        for (int i = 1; i < process_count; i++)
        {
            MPI_Recv(message, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            printf("Third round: %d receive a message from %d with tag %d: %s\n", my_rank, status.MPI_SOURCE, status.MPI_TAG, message);
            fflush(stdout);
        }
        printf("Message received from any source with any tag\n");
    }

    printf("Process %d of %d> DONE in the third round!\n", my_rank, process_count);
    // Ending of 2.c

    fflush(stdout);
    MPI_Finalize();
    free(message);
    return 0;
}