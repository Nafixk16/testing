/* C program for Merge Sort */
#include<stdlib.h>
#include<stdio.h>
#include <omp.h>
#include<sys/types.h>
#include<time.h>
// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
 
    #pragma omp parallel for 
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    
     
    #pragma omp  parallel for 
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    
   // printf("\nnum thread=>%d",omp_get_thread_num());
     
    
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
double mergeSort(int arr[], int l, int r)
{
   	
    double clock3, clock1,clock2,cpu_time_used;
  
int m = l+(r-l)/2;  
if (l < r)
{
	 clock3=omp_get_wtime();
         
	#pragma omp parallel
	{
		 #pragma omp  sections nowait
		 {
			#pragma omp section
			mergeSort(arr, l, m);
			#pragma omp section
			mergeSort(arr, m+1, r);
			
		}
			
	}
	
		
        
	
	merge(arr, l, m, r);
}
	clock1=omp_get_wtime(); 
	
       clock2=clock1-clock3;
       return clock2;
	 	 
    
}
 
/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size)
{
    int i;
    
    for (i=0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}
 
/* Driver program to test above functions */
int main()
{
    int arr[1000000]; 
    int arr_size = sizeof(arr)/sizeof(arr[0]);
     double clock2; 	
	for(int j=0;j<1000000;j++)
	{
		arr[j]=(rand()/1000000);
		
	}

   // printf("Given array is \n");

    // printArray(arr, arr_size);	 
     
   
     clock2=mergeSort(arr, 0, arr_size - 1);
    
   // printf("\nSorted array is \n");
      // printArray(arr, arr_size);	
    printf("\nclock 2  =>%f",clock2);
   
    printf("\n");
    return 0;
}
