// To compile the code: gcc -g -Wall -fopenmp -o histo openMP_histogram.c
// To run the code :  ./histo 5 0 50 5
// Input: bin_count_p, min_meas_p, max_meas_p, data_count_p
 
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Get_args(
      char*    argv[]        ,
      int*     bin_count_p   ,
      float*   min_meas_p    ,
      float*   max_meas_p    ,
      int*     data_count_p  );

void Gen_data(
      float   min_meas    , 
      float   max_meas    , 
      float   data[]      ,
      int     data_count  );

void Gen_bins(
      float min_meas      , 
      float max_meas      , 
      float bin_maxes[]   , 
      int   bin_counts[]  , 
      int   bin_count     );

int Which_bin(
      float    data        , 
      float    bin_maxes[] , 
      int      bin_count   , 
      float    min_meas    );

void Print_histo(
      float    bin_maxes[]   , 
      int      bin_counts[]  , 
      int      bin_count     , 
      float    min_meas      );


int main(int argc, char* argv[]) {
   int bin_count, i, bin;
   float min_meas, max_meas;
   float* bin_maxes;
   int* bin_counts;
   int data_count;
   float* data;
   int thread_count = omp_get_num_threads();

    
 
   Get_args(argv, &bin_count, &min_meas, &max_meas, &data_count);

    
   bin_maxes = malloc(bin_count*sizeof(float));
   bin_counts = malloc(bin_count*sizeof(int));
   data = malloc(data_count*sizeof(float));

    
   Gen_data(min_meas, max_meas, data, data_count);

    
   Gen_bins(min_meas, max_meas, bin_maxes, bin_counts, bin_count);

    
   #pragma omp parallel for num_threads(thread_count)
   for (i = 0; i < data_count; i++) {
      bin = Which_bin(data[i], bin_maxes, bin_count, min_meas);
      bin_counts[bin]++;
   }


   
   Print_histo(bin_maxes, bin_counts, bin_count, min_meas);

   free(data);
   free(bin_maxes);
   free(bin_counts);
   return 0;

}   
void Get_args(
      char*    argv[]        ,
      int*     bin_count_p   ,
      float*   min_meas_p    ,
      float*   max_meas_p    ,
      int*     data_count_p  ) {



   *bin_count_p = strtol(argv[1], NULL, 10);
   *min_meas_p = strtof(argv[2], NULL);
   *max_meas_p = strtof(argv[3], NULL);
   *data_count_p = strtol(argv[4], NULL, 10);

}   
void Gen_data(
        float   min_meas    , 
        float   max_meas    , 
        float   data[]      ,
        int     data_count  ) {
   int i;

   srandom(0);
    printf("\n Enter the elements:");
   #pragma omp parallel for
   for (i = 0; i < data_count; i++)
      scanf("%f",&data[i]);

} 
void Gen_bins(
      float min_meas      , 
      float max_meas      , 
      float bin_maxes[]   , 
      int   bin_counts[]  , 
      int   bin_count     ) {
   float bin_width;
   int   i;

   bin_width = (max_meas - min_meas)/bin_count;
   #pragma omp parallel for 
   for (i = 0; i < bin_count; i++) {
      bin_maxes[i] = min_meas + (i+1)*bin_width;
      bin_counts[i] = 0;
   }
} 
int Which_bin(
      float   data          , 
      float   bin_maxes[]   , 
      int     bin_count     , 
      float   min_meas      ) {
   int bottom = 0, top =  bin_count-1;
   int mid;
   float bin_max, bin_min;


   while (bottom <= top) {
      mid = (bottom + top)/2;
      bin_max = bin_maxes[mid];
      bin_min = (mid == 0) ? min_meas: bin_maxes[mid-1];
      if (data >= bin_max) 
         bottom = mid+1;
      else if (data < bin_min)
         top = mid-1;
      else
         return mid;
   }

    
   fprintf(stderr, "Data = %f does not belong to any bin\n", data);
   fprintf(stderr, "Exiting\n");
   exit(-1);
}   
void Print_histo(
        float  bin_maxes[]   , 
        int    bin_counts[]  , 
        int    bin_count     , 
        float  min_meas      ) {
    int i;
   float bin_max, bin_min;

   for (i = 0; i < bin_count; i++) {
      bin_max = bin_maxes[i];
      bin_min = (i == 0) ? min_meas: bin_maxes[i-1];
	   printf("The number of elements in bin %d from %.3f to %.3f: \t",i,bin_min, bin_max);

		printf("%d",bin_counts[i]);
      printf("\n");
   }
}   