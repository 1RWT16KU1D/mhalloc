// Self-implementation of malloc()
#include "mhalloc.h"
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>

void *mhalloc(size_t size)
{
    if (size == 0)
        return NULL;

    void *ptr = sbrk(size);
    if (ptr == NULL)
        return NULL;
    
    return ptr;
}

void __attribute__((unused)) mhfree(void *ptr)
{

}