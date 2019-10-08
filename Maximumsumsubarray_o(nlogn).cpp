#include <stdio.h> 
#include <limits.h> 
  int max(int a, int b) { return (a > b)? a : b; } 
  int max(int a, int b, int c) { return max(max(a, b), c); } 
  
  int maximumCrossingSum(int arr[], int l, int m, int h) 
     {
    int sum = 0; 
    int leftsum = INT_MIN; 
    for (int i = m; i >= l; i--) 
    { 
        sum = sum + arr[i]; 
        if (sum > leftsum) 
          leftsum = sum; 
    }
    sum = 0; 
    int rightsum = INT_MIN; 
    for (int i = m+1; i <= h; i++) 
    { 
         sum = sum + arr[i]; 
        if (sum > rightsum) 
          rightsum = sum; 
    } 
    return leftsum + rightsum; 
}
int maxSubArraySum(int arr[], int l, int h) 
{ 
    if (l == h) 
     return arr[l]; 
  int m = (l + h)/2; 
  return max(maxSubArraySum(arr, l, m), 
              maxSubArraySum(arr, m+1, h), 
              maximumCrossingSum(arr, l, m, h)); 
}
  int main() 
{ int n;
int arr[100];
    printf("\nEnter the number of elements");
    scanf("%d",&n);
    printf("Enter the Array Elements");
   for(int i=0;i<n;i++)
   {
       scanf("%d",&arr[i]);
   }
   int maximumsum = maxSubArraySum(arr, 0, n-1); 
   printf("\n The Maximum continuous sum is %d \n", maximumsum); 
   getchar(); 
   return 0; 
} 