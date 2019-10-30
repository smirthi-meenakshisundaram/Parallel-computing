// compile: mpicc -g -Wall -o histo mpi_histogram.c
//execute: mpiexec -n 2  histo
//  Input : Gave the number of bins as 5
// Input : Gave the minimum measurement as 1
// Input : Gave the maximum measurement as 50
// Input : Gave the bin count 8

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Get_input(   int* bin_count_p,
                 float* min_meas_p,
                  float* max_meas_p,
                  int* data_count_p,
                  int* local_data_count_p,
                  int my_rank,
                  int comm_sz,
                  MPI_Comm comm);

void Generation_data(    float local_data[],
                  int local_data_count,
                  int data_count,
                  float min_meas,
                  float max_meas,
                  int my_rank,
                  MPI_Comm comm);

void Settingup_bins(    float bin_maxes[],
                  int loc_bin_cts[],
                  float min_meas,
                  float max_meas,
                  int bin_count,
                  int my_rank,
                  MPI_Comm comm);

void Find_bins(   int bin_counts[],
                  float local_data[],
                  int loc_bin_cts[],
                  int local_data_count,
                  float bin_maxes[],
                  int bin_count,
                  float min_meas,
                  MPI_Comm comm);

int Which_bin(    float data,
                  float bin_maxes[],
                  int bin_count,
                  float min_meas);

void Printing_histo( float bin_maxes[],
                  int bin_counts[],
                  int bin_count,
                  float min_meas);

void e(           int error);

int main(int argc, char* argv[]) {
  int       bin_count;
  float     min_meas;
  float     max_meas;
  float*    bin_maxes;
  int*      bin_counts;
  int*      loc_bin_cts;
  int       data_count;
  int       local_data_count;
  float*    data;
  float*    local_data;
  int       my_rank;
  int       comm_sz;
  MPI_Comm  comm;

  e(MPI_Init(&argc, &argv));
  comm = MPI_COMM_WORLD;
  e(MPI_Comm_size(comm, &comm_sz));
  e(MPI_Comm_rank(comm, &my_rank));

  
  Get_input(&bin_count, &min_meas, &max_meas, &data_count,
            &local_data_count, my_rank, comm_sz, comm);

  
  bin_maxes = malloc(bin_count*sizeof(float));
  bin_counts = malloc(bin_count*sizeof(int));
  loc_bin_cts = malloc(bin_count*sizeof(int));
  data = malloc(data_count*sizeof(float));
  local_data = malloc(local_data_count*sizeof(float));

  
  Settingup_bins(bin_maxes,loc_bin_cts,min_meas,max_meas,bin_count,my_rank,comm);
  Generation_data(local_data,local_data_count,data_count,min_meas,max_meas,my_rank,comm);
  Find_bins(bin_counts,local_data,loc_bin_cts,local_data_count,bin_maxes,bin_count,min_meas,comm);
  e(MPI_Reduce(loc_bin_cts,bin_counts,bin_count,MPI_INT,MPI_SUM,0,comm));
  if(my_rank == 0)
    Printing_histo(bin_maxes,bin_counts,bin_count,min_meas);

  free(bin_maxes);
  free(bin_counts);
  free(loc_bin_cts);
  free(data);
  free(local_data);
  MPI_Finalize();
  return 0;
} 

void e(int error) {
  if(error != MPI_SUCCESS) {
    fprintf(stderr,"Error starting MPI program, Terminating.\n");
    MPI_Abort(MPI_COMM_WORLD,error);
    MPI_Finalize();
    exit(1);
  }
}

void Printing_histo(
     float bin_maxes[] ,
     int bin_counts[]  ,
     int bin_count     ,
     float min_meas    ) {
  int max = 0;
  int i;

  for(i = 0; i < bin_count; i++) {
    if(bin_counts[i] > max)
      max = bin_counts[i];
  }
  for(i = 0; i < bin_count; i++) {
    printf("\n The number of values in bin %d with bin_maxes %10.3f:",i+1,bin_maxes[i]); 
    printf("  %d\n",bin_counts[i]);
  }
}  
void Find_bins(
     int bin_counts[]      ,
     float local_data[]    ,
     int loc_bin_cts[]     ,
     int local_data_count  ,
     float bin_maxes[]     ,
     int bin_count         ,
     float min_meas        ,
     MPI_Comm comm){

  int i;
  int bin;

  for(i = 0; i < local_data_count; i++) {
    bin = Which_bin(local_data[i],bin_maxes,bin_count,min_meas);  
    loc_bin_cts[bin]++;
  }
}  

int Which_bin(float data, float bin_maxes[], int bin_count,
     float min_meas) {

  int i;
  for(i = 0; i < bin_count-1; i++) {
    if(data <= bin_maxes[i]) break;
  }
  return i;
}  
void Settingup_bins(
     float bin_maxes[]  ,
     int loc_bin_cts[]  ,
     float min_meas     ,
     float max_meas     ,
     int bin_count      ,
     int my_rank        ,
     MPI_Comm comm      ) {

  float range = max_meas - min_meas;
  float interval = range / bin_count;

  int i;
  for(i = 0; i < bin_count; i++) {
    bin_maxes[i] = interval * (float)(i+1) + min_meas;
    loc_bin_cts[i] = 0;
  }
}  
void Generation_data(
     float local_data[]    ,
     int local_data_count  ,
     int data_count        ,
     float min_meas        ,
     float max_meas        ,
     int my_rank           ,
     MPI_Comm comm         ) {
  float* data;
  if(my_rank == 0) {
    float range = max_meas - min_meas;
    data = malloc(data_count*sizeof(float));

    int i;
    
    for(i=0;i<data_count;i++) {
      data[i] = (float) rand() / (float) RAND_MAX * range + min_meas;
      
    }
  }
  e(MPI_Scatter(data,local_data_count,MPI_FLOAT,local_data,local_data_count,MPI_FLOAT, 0, comm));
  if(my_rank == 0) free(data);
}  


void Get_input(
     int* bin_count_p,        
     float* min_meas_p,       
     float* max_meas_p,       
     int* data_count_p,       
     int* local_data_count_p, 
     int my_rank,             
     int comm_sz,             
     MPI_Comm comm) {

  if(my_rank == 0) {
    printf("Number of bins (int): ");
    scanf("%d",bin_count_p);
    printf("Minimum value (float): ");
    scanf("%f",min_meas_p);
    printf("Maximum value (float): ");
    scanf("%f",max_meas_p);
    
    if(*max_meas_p < *min_meas_p) {
      float* temp = max_meas_p;
      max_meas_p = min_meas_p;
      min_meas_p = temp;
    }
    printf("Number of values (int): ");
    scanf("%d",data_count_p);
    *local_data_count_p = *data_count_p / comm_sz;
    *data_count_p = *local_data_count_p * comm_sz;
    printf("\n");
  }
  e(MPI_Bcast(bin_count_p,1,MPI_INT,0,comm));
  e(MPI_Bcast(min_meas_p,1,MPI_FLOAT,0,comm));
  e(MPI_Bcast(max_meas_p,1,MPI_FLOAT,0,comm));
  e(MPI_Bcast(data_count_p,1,MPI_INT,0,comm));
  e(MPI_Bcast(local_data_count_p,1,MPI_INT,0,comm));
}  