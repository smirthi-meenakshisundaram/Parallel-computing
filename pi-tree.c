#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
long thread_count;
long long n;
long block_size = 0;
double sum;
double *thread_sum = NULL;
int *counts;         
sem_t *count_sems;   // semaphore for count
sem_t *barrier_sems; // semaphore for barrier

void *Thread_sum_pi(void *rank);
void Get_args(int argc, char *argv[]);
void Usage(char *prog_name);
double Serial_pi(long long n);
int main(int argc, char *argv[])
{
    long thread;
    pthread_t *thread_handles;
    Get_args(argc, argv);
    thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
    
    thread_sum = malloc(thread_count * sizeof(long));
    block_size = n % thread_count == 0 ? n / thread_count : n / thread_count + 1;
    
    
    int levels = ceil(log(thread_count) / log(2)) + 1;
    counts = malloc(levels * sizeof(int));
    for (int i = 0; i < levels; i++)
        counts[i] = 0;
    count_sems = malloc(levels * sizeof(sem_t));
    for (int i = 0; i < levels; i++)
        sem_init(&(count_sems[i]), 0, 1); 
    barrier_sems = malloc(levels * sizeof(sem_t));
    for (int i = 0; i < levels; i++)
        sem_init(&(barrier_sems[i]), 0, 0); 

    sum = 0.0;
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Thread_sum_pi, (void *)thread);
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    thread_sum[0] = 4.0 * thread_sum[0];
    printf("With n = %lld terms,\n", n);
    printf("   Our estimate of pi = %.15f\n", thread_sum[0]);
    sum = Serial_pi(n);
    printf("   Single thread est  = %.15f\n", sum);

    printf("The estimate of pi = %.15f\n", 4.0 * atan(1.0));

    free(thread_handles);
    free(counts);
    for (int i = 0; i < levels + 1; i++)
        sem_destroy(&(barrier_sems[i]));
    for (int i = 0; i < levels + 1; i++)
        sem_destroy(&(count_sems[i]));
    return 0;
}

void *Thread_sum_pi(void *rank)
{
    long my_rank = (long)rank;
    double sign;
    long long i;
    thread_sum[my_rank] = 0;
    long my_first_id = block_size * my_rank;
    long my_last_id = block_size * (my_rank + 1) > n ? n : block_size * (my_rank + 1);
    //double my_sum = 0.0;

    if (my_first_id % 2 == 0)
        sign = 1.0;
    else
        sign = -1.0;

    for (i = my_first_id; i < my_last_id; i++, sign = -sign)
    {
        thread_sum[my_rank] += (sign / (2 * i + 1));
        
    }
    
    printf("Local sum of thread %d from %ld to %ld is equal to %ld\n", my_rank, my_first_id, my_last_id, thread_sum[my_rank]);
    int iLevel = 0;
    for (int jumps = 1; jumps < thread_count; jumps *= 2)//Tree structure code to parallelise the addition (Global sum)
    {
        if (my_rank % (iLevel == 0 ? 1 : 2 * iLevel) == 0)
        {
            int active_threads = iLevel == 0 ? thread_count : floor(thread_count / (2 * iLevel));
            sem_wait(&count_sems[iLevel]);
            if (counts[iLevel] == active_threads - 1)
            {
                counts[iLevel] = 0;
                sem_post(&count_sems[0]);
                for (int j = 0; j < active_threads - 1; j++)
                    sem_post(&barrier_sems[iLevel]);
            }
            else
            {
                counts[iLevel]++;
                sem_post(&count_sems[iLevel]);
                sem_wait(&barrier_sems[iLevel]);
                printf("In level %d, %d is released from waiting...\n", iLevel, my_rank);
                fflush(stdout);
            }
            if (my_rank == 0)
            {
                printf("%d threads completed barrier %d \n", active_threads, iLevel);
                fflush(stdout);
            }
            if (my_rank % (2 * jumps) == 0)
                thread_sum[my_rank] += thread_sum[my_rank + jumps];
        }
        iLevel++;
    }
    return NULL;
    
}

double Serial_pi(long long n)
{
    double sum = 0.0;
    long long i;
    double sign = 1.0;

    for (i = 0; i < n; i++, sign = -sign)
    {
        sum += sign / (2 * i + 1);
    }
    return 4.0 * sum;
}

void Get_args(int argc, char *argv[])
{
    if (argc != 3)
        Usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0)
        Usage(argv[0]);
    n = strtoll(argv[2], NULL, 10);
    if (n <= 0)
        Usage(argv[0]);
}

void Usage(char *prog_name)
{
    fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
    fprintf(stderr, "   n is the number of terms and should be >= 1\n");
    fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
    exit(0);
}