#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// calculates the minimum value
int min(int *n1, int *n2, int *n3, int *n4)
{
    int min = *n1;
    if (*n2 < min)
        min = *n2;
    if (*n3 < min)
        min = *n3;
    if (*n4 < min)
        min = *n4;
    return min;
}

// calculates the maximum value
int max(int *n1, int *n2, int *n3, int *n4)
{
    int max = *n1;
    if (*n2 > max)
        max = *n2;
    if (*n3 > max)
        max = *n3;
    if (*n4 > max)
        max = *n4;
    return max;
}

// product of the results of the two methods
int prod(int (*min)(int *, int *, int *, int *), int (*max)(int *, int *, int *, int *), int *n1, int *n2, int *n3, int *n4)
{
    return min(n1, n2, n3, n4) * max(n1, n2, n3, n4);
}

int main()
{
    srand(time(0));
    // random values assigned
    int n1 = rand() % 100;
    int n2 = rand() % 100;
    int n3 = rand() % 100;
    int n4 = rand() % 100;


    printf("n1: %d, n2: %d, n3: %d n4:%d\n", n1, n2, n3, n4);
    // print the result
    int result = prod(min, max, &n1, &n2, &n3, &n4);
    printf("The product of min and max is: %d\n", result);
    return 0;
}