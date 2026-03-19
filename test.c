#include "mhalloc.h"
#include <stdio.h>

#define EXIT_FAILURE 1 // I don't want to include stdlib.h just for this
#define safe_mhalloc(size, type, ptr) \
ptr = (type *)mhalloc(size * sizeof(type)); \
if (ptr == NULL) \
{ \
    fprintf(stderr, "Couldn't allocate memory!\n"); \
    return EXIT_FAILURE; \
} \

static inline void printArray(int arr[], int num)
{
    for (int i = 0; i < 10; i++) printf("arr%d[%d]: %d\n", num, i, arr[i]);
    printf("\n");
}

int main(void)
{
    int *arr1, *arr2;
    safe_mhalloc(10, int, arr1);
    safe_mhalloc(10, int, arr2);

    for (int i = 0; i < 10; i++)
    {
        arr1[i] = i + 1;
        arr2[i] = 10 * (i + 1);
    }

    printArray(arr1, 1);
    printArray(arr2, 2);

    printf("Address of arr1: %p\n", arr1);
    printf("Address of arr2: %p\n", arr2);

    mhfree(arr1);
    int *arr3;
    safe_mhalloc(10, int, arr3);

    for (int i = 0; i < 10; i++) arr3[i] = i + 2;
    printArray(arr2, 2);
    printArray(arr3, 3);
    
    printf("Address of arr3: %p\n", arr3);

    if (arr1 == arr3)
        printf("Test passed!\n");
    else
        printf("Test failed!\n");
    return 0;
}