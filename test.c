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

    for (int i = 0; i < 10; i++) printf("arr1[%d]: %d\n", i, arr1[i]);
    printf("\n");
    for (int i = 0; i < 10; i++) printf("arr2[%d]: %d\n", i, arr2[i]);

    printf("Test passed!\n");
    return 0;
}