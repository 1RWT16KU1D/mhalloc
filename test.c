#include "mhalloc.h"
#include <stdio.h>

#define EXIT_FAILURE 1
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
    int *arr1, *arr2, *arr3, *arr4, *arr5, *arr6;

    // Basic allocation
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

    printf("Address of arr1: %p\n", (void *)arr1);
    printf("Address of arr2: %p\n", (void *)arr2);
    printf("Address of arr3: %p\n", (void *)arr3);

    // Reuse test — free arr2, allocate arr4, should reuse arr2's block
    mhfree(arr2);
    safe_mhalloc(10, int, arr4);
    for (i = 0; i < 10; i++) arr4[i] = 20 * (i + 1);

    printArray(arr1, 1);
    printArray(arr3, 3);
    printArray(arr4, 4);

    printf("Address of arr4: %p\n", (void *)arr4);
    printf("Reuse test: %s\n", arr2 == arr4 ? "PASSED" : "FAILED");

    // Splitting test — free arr4, allocate smaller block, should split
    mhfree(arr4);
    safe_mhalloc(4, int, arr5); // Half the size, should split
    printf("Address of arr5: %p\n", (void *)arr5);
    printf("Splitting test: %s\n", arr5 == arr4 ? "PASSED" : "FAILED");

    // Coalescing test — free arr1 and arr3, then free arr5 (between them)
    // All three should merge into one block
    mhfree(arr1);
    mhfree(arr3);
    mhfree(arr5);
    safe_mhalloc(30, int, arr6); // Should fit in the coalesced block
    printf("Address of arr6: %p\n", (void *)arr6);
    printf("Coalescing test: %s\n", arr6 == arr1 ? "PASSED" : "FAILED");

    // mhcalloc test — all elements should be zero
    int *arr7 = mhcalloc(10, sizeof(int));
    printArray(arr7, 7);
    int calloc_passed = 1;
    for (i = 0; i < 10; i++)
        if (arr7[i] != 0) { calloc_passed = 0; break; }
    printf("mhcalloc() test: %s\n", calloc_passed ? "PASSED" : "FAILED");

    return 0;
}