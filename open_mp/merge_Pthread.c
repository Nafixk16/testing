
#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"
#include "string.h"
#include "math.h"

struct Node {
   int value;
   struct Node *next;
};

struct List {
   int size;
   struct Node *first;
};

struct ThreadInfo {
   char *threadName;
   int leftBound;
   int rightBound;
};

// init and create a new List structure
struct List createList(){
   struct List l;
   l.size = 0;
   l.first = NULL;
   return l;
}

// appends an integer value to the passed list structure
void addItem(struct List *list, int item){
   if (list->size == 0){
      list->first = (struct Node *) malloc(sizeof(struct Node));
      list->first->value = item;
      list->first->next = NULL;
   }
   else {
      struct Node *node = list->first; 
      while(node->next != NULL){
         node = node->next;
      }
      node->next = (struct Node *) malloc(sizeof(struct Node));
      node->next->value = item;
      node->next->next = NULL;
   }
   list->size++;
}

//returns the first item in the passed list
int getFirstItem(struct List *list){
   return list->first->value;
}

// removes and returns the first item from the passed list
int removeFirstItem(struct List *list){
   struct Node *firstNode = list->first;
   int returnValue = firstNode->value;
   list->first = firstNode->next;
   free(firstNode);
   list->size--;
   return returnValue;
}

int *list;

char *printListSegment(int *array, int i, int j){
   char *result = (char*) malloc(2);
   result = strcpy(result, "[");
   char dummy [50]; // temporarily stores <toConcat> to obtain length   

   for (; i < j; i++) {
      result = (char*) realloc(result, (sizeof(char) * (strlen(result) + 1)) + 
         (sizeof(char) * (sprintf(dummy, "%d", array[i]) + 1)));
      sprintf(result, "%s%d", result, array[i]);
      if (i + 1 != j) {
         result = (char*) realloc(result, (sizeof(char) * (strlen(result) + 1)) +
            (sizeof(char) * 3));
         sprintf(result, "%s, ", result);
      }
   }
   result = (char*) realloc(result, (sizeof(char) * (strlen(result) + 1)) +
      (sizeof(char) * 2));
   sprintf(result, "%s]", result);
   return result;
}

void merge(int *array, int i, int middle, int j){
   struct List left = createList();
   struct List right = createList();
   struct List result = createList();
   int arrayI;
   // populate left list
   for (arrayI = i; arrayI < middle; arrayI++){
      addItem(&left, array[arrayI]);
   }
   // populate right list
   for (arrayI = middle; arrayI < j; arrayI++){
      addItem(&right, array[arrayI]);
   }
   
   // the heart of the mergeSort
   while (left.size > 0 || right.size > 0){
      if (left.size > 0 && right.size > 0) {
         if (getFirstItem(&left) <= getFirstItem(&right)){
            addItem(&result, removeFirstItem(&left));
         }
         else {
            addItem(&result, removeFirstItem(&right));
         }
      }
      else if (left.size > 0){
         addItem(&result, removeFirstItem(&left));
      }
      else if (right.size > 0){
         addItem(&result, removeFirstItem(&right));
      }
   }

   // throw the elements of List result into the actual passed array
   for (; i < j; i++){
      array[i] = removeFirstItem(&result);
   }

}

void *mergeSortThread (void *ptr) {
   struct ThreadInfo *info = (struct ThreadInfo*) ptr;
 
   printf("Thread %s to sort indices [%d, %d); Elements: %s\n", 
      info->threadName, info->leftBound, info->rightBound,
      printListSegment(list, info->leftBound, info->rightBound));
   
    if (info->rightBound > info->leftBound + 1) { // segment has more than one element
      int middle = (int) (((info->rightBound - info->leftBound) / 2) + info->leftBound);
      pthread_t leftThread, rightThread;

      // create ThreadInfo for "left" thread
      struct ThreadInfo *leftInfo = (struct ThreadInfo*) malloc(sizeof(struct ThreadInfo));
      leftInfo->threadName = (char*) malloc((sizeof(char) * (strlen(info->threadName) + 3)));
      sprintf(leftInfo->threadName, "%sa", info->threadName);
      leftInfo->leftBound = info->leftBound;
      leftInfo->rightBound = middle;

      // create ThreadInfo for "right" thread
      struct ThreadInfo *rightInfo = (struct ThreadInfo*) malloc(sizeof(struct ThreadInfo));      
      rightInfo->threadName = (char*) malloc((sizeof(char) * (strlen(info->threadName) + 3)));
      sprintf(rightInfo->threadName, "%sb", info->threadName);
      rightInfo->leftBound = middle;
      rightInfo->rightBound = info->rightBound;

      pthread_create(&leftThread, NULL, mergeSortThread, (void*) leftInfo);
      pthread_create(&rightThread, NULL, mergeSortThread, (void*) rightInfo);

     // pthread_join(leftThread, NULL);
      //pthread_join(rightThread, NULL);

      merge(list, info->leftBound, middle, info->rightBound);
   }

   //printf("Thread %s sorted indices [%d, %d); Elements: %s\n",
     // info->threadName, info->leftBound, info->rightBound,
   //   printListSegment(list, info->leftBound, info->rightBound));
}

int main()
{
   ///struct timespec begin, end;
   double elapsed;
   int numInts, i;
   clock_t start = clock();
   clock_t end = clock();
   pthread_t thread1;
   printf("Enter number of integers to be sorted: ");
   scanf("%d", &numInts);
   printf("\n");
   list = (int*) malloc(sizeof(int) * numInts+1);
   for (i = 0; i < numInts; i++)
    {
        list[i]=(rand()/1000000);
   }
  // printf("Array before sorting: \n%s\n", 
    //  printListSegment(list, 0, numInts));

   struct ThreadInfo *info = (struct ThreadInfo*) malloc(sizeof(struct ThreadInfo));
   info->threadName = (char*) malloc(2); // "a\0"
   info->threadName = strcpy(info->threadName, "a");
   info->leftBound = 0;
   info->rightBound = numInts;
   
//	clock_gettime(CLOCK_MONOTONIC, &begin);
start = clock();
   pthread_create(&thread1, NULL, mergeSortThread, (void*) info);
   pthread_join(thread1, NULL);
	
	//clock_gettime(CLOCK_MONOTONIC, &end);
end=clock();
	//elapsed = end.tv_sec - begin.tv_sec;
elapsed=(double)( (end - start))/ CLOCKS_PER_SEC;	
//elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
	printf("\ncpu => %f",elapsed);

  // printf("\nArray after sorting: \n%s\n",
    //  printListSegment(list, 0, numInts));
   return 0;
}

