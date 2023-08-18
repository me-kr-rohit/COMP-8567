#include<stdio.h>  
#include<stdlib.h>  
#include<stdint.h> 
#include<time.h>  
#include<pthread.h> 
#define SIZE_OF_ARRAYRK 100  

#generating the random interger between(10 and 20)
int createRandomrk() 
{  
    return rand() % 11 + 10;  
}  

//to provide the sum
void* arraySumrk(void* argrk)
{  
    int* arrayrk = (int*)argrk;  
    int sumVal = 0;  
    for (int i = 0; i < SIZE_OF_ARRAYRK; ++i) 
	{  
        sumVal += arrayrk[i];  
    }  
    //exit the thread and return the sum as a pointer (void *)  
    pthread_exit((void*)(intptr_t)sumVal);  
}  

#
int main() 
{  
   
   //to initialize the array
    srand(time(NULL));  
    int ia1[SIZE_OF_ARRAYRK];
	int ia2[SIZE_OF_ARRAYRK];
	int ia3[SIZE_OF_ARRAYRK]; 
	
	//to create random number
   for (int i = 0; i < SIZE_OF_ARRAYRK; ++i) 
   {  
        ia1[i] = createRandomrk();  
        ia2[i] = createRandomrk();  
        ia3[i] = createRandomrk();  
    }  

	//to create threads
    pthread_t rktid1, rktid2, rktid3;  
    pthread_create(&rktid1, NULL, arraySumrk, (void*)ia1);  
    pthread_create(&rktid2, NULL, arraySumrk, (void*)ia2);  
    pthread_create(&rktid3, NULL, arraySumrk, (void*)ia3);  

    void* retVal1rk;  
    void* retVal2rk;  
    void* retVal3rk;  

	//created thread join to wait for threads to complete
    pthread_join(rktid1, &retVal1rk);  
    pthread_join(rktid2, &retVal2rk);  
    pthread_join(rktid3, &retVal3rk);  

	//tostore the sum in variables
    int sumrk1 = (int)(intptr_t)retVal1rk;  
    int sumrk2 = (int)(intptr_t)retVal2rk;  
    int sumrk3 = (int)(intptr_t)retVal3rk;  

    //to print the sums  
    printf("Sum of array 1: %d\n", sumrk1);  
    printf("Sum of array 2: %d\n", sumrk2);  
    printf("Sum of array 3: %d\n", sumrk3);  

    //calculate & print the sum of all elements  
   int valrk= sumrk1 + sumrk2 + sumrk3;  
   printf("sum of all elements: %d\n", valrk);  
    return 0;  
}  
