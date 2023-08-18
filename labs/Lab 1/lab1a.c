#include <stdio.h>
#include <stdlib.h>
int main(void) 
{
    int *ar,n;
    printf("\nEnter the number of elements in the array\n");
    scanf("%d",&n);
    ar=malloc(n*sizeof(int));
    for(int i=0;i<n;i++)
    {
        printf("Enter element %d\n", i);
        scanf("%d",&ar[i]);
    }

    // printing using de-referencing
    printf("\nThe elements of the array using de-referencing\n");
    for(int i = n - 1; i >= 0; i--)
    {
        printf("\n%d", *(ar + i));
    }
    free(ar);
}