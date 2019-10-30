    /* File:    trap.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  Estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -o trap trap_openmp1.c
 * Usage:   ./trap
 *
 * Note:    The function f(x) is hardwired.
 *
 * IPP:     Section 3.2.1 (pp. 94 and ff.) and 5.2 (p. 216)
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x);    /* Function we're integrating */
void Trap(double a, double b, int n, double*global_result_p);

int main(int argc, char *argv[]) {
   double global_result = 0.0;
  // double  integral;   /* Store result in integral   */
   double  a, b;       /* Left and right endpoints   */
   int     n;          /* Number of trapezoids       */
  // double  h;            /* Height of trapezoids       */
 int thread_count;      
 thread_count = strtol(argv[1],NULL,10);
   printf("Enter a, b, and n\n");
   scanf("%lf", &a);
   scanf("%lf", &b);
   scanf("%d", &n);
   # pragma omp parallel num_threads(thread_count)
   Trap(a,b,n,&global_result);

   //h = (b-a)/n;
   //integral = Trap(a, b, n, h);
   
   printf("With n = %d trapezoids, our estimate ", n);
   printf("of the integral from %.2f to %.2f = %.14e\n",a, b, global_result);

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Estimate integral from a to b of f using trap rule and
 *              n trapezoids
 * Input args:  a, b, n, h
 * Return val:  Estimate of the integral 
 */
void Trap(double a, double b, int n, double*global_result_p) {
   double h, x, my_result;
   double local_a, local_b;
   int i, local_n;
int my_rank = omp_get_thread_num();
int thread_count = omp_get_num_threads();

h=(b-a)/n;
local_n = n/thread_count;
local_a = a + my_rank*local_n*h;
local_b = local_a + local_n*h;
my_result = (f(local_a) + f(local_b))/2.0;
for(i=1;i<=local_n-1;i++)
{
   x=local_a + i*h;
   my_result += f(x);
}
my_result = my_result*h;
# pragma omp critical
*global_result_p += my_result;
}
double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */