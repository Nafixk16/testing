#include<stdlib.h>
#include<stdio.h>
#include<time.h> 
#include<pthread.h>

pthread_t thread1,thread2;
struct parameter
{

	int m;
	int r;
	long arr[100000];
	int l;

};
void merge(long arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
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
//printf("\n%d",arr[k]);
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
void * mergeSort(void *vptr/*int arr[], int l, int r*/)
{
 	double cpu_time_used;
	
	
          struct parameter data;
	  data = *(struct parameter*)vptr;

	  struct parameter data2;
	  data2 = *(struct parameter*)vptr;

   	 struct parameter data3;
	  data3 = *(struct parameter*)vptr;
	clock_t start ;
	
	clock_t end ;
	
//printf("\nhere=> %d   %d",data.r,data.l);
//printf("\n");
    if (data.l < data.r)
    {
	//printf("nafes	");
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        data.m = data.l+(data.r-data.l)/2;
        data2.l=data.l;
        data2.m=data.m+1;
	data2.r=data.m; 
	
      //  mergeSort(arr, l, m);
	  
	data3.l=data.m;
	data3.r=data.r;

	//clock_gettime(CLOCK_MONOTONIC, &begin);
//start=clock();
	pthread_create(&thread1,0, mergeSort,&data2);
	pthread_create(&thread2,0, mergeSort,&data3);
       // mergeSort(arr, m+1, r);
       
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	
        merge(data.arr, data.l, data.m, data.r);
	//clock_gettime(CLOCK_MONOTONIC, &end);
//end=clock();
	//elapsed = end.tv_sec - begin.tv_sec;
	//elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
	
    }
	//end =clock();
	//cpu_time_used = (double)( (end - start))/ CLOCKS_PER_SEC; 
	//	printf("\ncpu => %f",cpu_time_used); 
     
	
}
 
/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(long A[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%lo ", A[i]);
    printf("\n");
}
 
/* Driver program to test above functions */
int main()
{	
        int j;  
	pthread_t thread3;
        struct parameter data1;
	
	
	double cpu_time_used;
	clock_t start, end;
        int arr_size = sizeof(data1.arr)/sizeof(data1.arr[0]);
 
	for(j=0;j<100000;j++)
	{
		data1.arr[j]=(rand()/1000000);
		
	}

       printf("Given array is \n");  
   //  printArray(arr, arr_size);	
    	 
	start=clock();
     pthread_create(&thread3,0, mergeSort,&data1);
     pthread_join(thread3,NULL);
	//mergeSort(&data1);	
end=clock();
cpu_time_used = (double)( (end - start))/ CLOCKS_PER_SEC; 
		printf("\ncpu => %f",cpu_time_used); 
    printf("\nSorted array is \n");
   // printArray(data1.arr, arr_size);
//	printf("\n clock->%f",cpu_time_used);
	printf("\n");
    return 0;
}
