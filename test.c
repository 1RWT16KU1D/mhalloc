#include "mhalloc.h"
#include <stdio.h>

#define EXIT_FAILURE 1 // I don't want to include stdlib.h just for this
#define safe_mhalloc(size, type, ptr) \
ptr = (type *)mhalloc(size * sizeof(type)); \
if (ptr == NULL) \
{ \
    fprintf(stderr, "Couldn't allocate memory!\n"); \
    return EXIT_FAILURE; \
}

static int i = 0;

static inline void printArray(int arr[], int num)
{
    for (i = 0; i < 10; i++)
        printf("arr%d[%d]: %d\n", num, i, arr[i]);
}

int main(void)
{
    int *arr1, *arr2, *arr3, *arr4;
    safe_mhalloc(10, int, arr1);
    safe_mhalloc(10, int, arr2);
    safe_mhalloc(10, int, arr3);

    for (i = 0; i < 10; i++)
    {
        arr1[i] = i + 1;
        arr2[i] = 10 * (i + 1);
        arr3[i] = 100 * (i + 1);
    }

    printArray(arr1, 1);
    printArray(arr2, 2);
    printArray(arr3, 3);

    printf("Address of arr1: %p\n", arr1);
    printf("Address of arr2: %p\n", arr2);
    printf("Address of arr3: %p\n", arr3);

    mhfree(arr2);
    safe_mhalloc(10, int, arr4);

    for (i = 0; i < 10; i++) arr4[i] = 20 * (i + 1);
    printArray(arr1, 1);
    printArray(arr3, 3);
    printArray(arr4, 4);
    
    printf("Address of arr4: %p\n", arr4);

    if (arr2 == arr4)
        printf("Test passed!\n");
    else
        printf("Test failed!\n");

    printf("mhcalloc() test:\n");
    int *arr5 = mhcalloc(10, sizeof(int));

    printArray(arr5, 5);
    if (arr5[5] == 0)
        printf("Test passed!\n");
    else
        printf("Test failed!\n");
    return 0;
}