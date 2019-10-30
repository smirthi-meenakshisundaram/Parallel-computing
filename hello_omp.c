
     /* File:   hello_serial.c
 *
 * Purpose:  A serial hello, world program
 *
 * Compile:  gcc -g -Wall  -o hello_serial hello_serial.c
 * Run:      ./hello_serial.c
 * 
 * Input:    none
 * Output:   A greeting message
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello(void);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc,char*argv[]) 
{
    if (argc<1)
    
        printf("Usage: hello_serial ${num_threads}");
    
   
   int thread_count = strtol(argv[1], NULL, 10);
    #pragma omp parallel num_threads(thread_count)
 
  Hello();

return 0; 
}  /* main */

/*-------------------------------------------------------------------
 * Function:    Hello
 * Purpose:     A function that prints message
 */
void Hello(void) {
     int my_rank =omp_get_thread_num();
     int thread_count = omp_get_num_threads();

   printf("Hello ! from %d of %d \n",my_rank,thread_count);

}  /* Hello */