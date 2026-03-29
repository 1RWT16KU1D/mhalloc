// Self-implementation of malloc(), free(), calloc(), realloc()
#include "mhalloc.h"
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

// Metadata
typedef struct mhblock
{
    size_t size;
    bool free;
    struct mhblock *next;
} mhblock_t;

static mhblock_t *head = NULL; // Static to initialise values to 0

#define METADATA_SIZE sizeof(mhblock_t)

#pragma region Core Functions
static inline bool isBlockUsable(mhblock_t *block, size_t size) // Could've used a macro for this but meh
{
    return block->free && block->size >= size;
}

void *mhalloc(size_t size)
{
    if (size == 0)
        return NULL;

    // Align size to 8 bytes
    size = (size + 7) & ~7;

    // Initialise first block metadata
    if (head == NULL)
    {
        mhblock_t *ptr = (mhblock_t *)sbrk(METADATA_SIZE + size);
        if ((void *)ptr == (void *)-1)
            return NULL;

        head = ptr;

        head->size = size;
        head->free = false;
        head->next = NULL;

        return (void *)(ptr + 1);
    }
    else // Assign only suitable block
    {
        mhblock_t *curr = head;
        mhblock_t *prev = NULL;

        while (curr)
        {
            if (isBlockUsable(curr, size))
            {
                curr->free = false;
                return (void *)(curr + 1);
            }

            prev = curr;
            curr = curr->next;
        }

        // Suitable block not found, create new block
        mhblock_t *ptr = (mhblock_t *)sbrk(METADATA_SIZE + size);
        if ((void *)ptr == (void *)-1)
            return NULL;

        prev->next = ptr;

        ptr->size = size;
        ptr->free = false;
        ptr->next = NULL;

        return (void *)(ptr + 1);
    }
}

void mhfree(void *ptr)
{
    if (ptr) // Free pointer only if valid
        ((mhblock_t *)ptr - 1)->free = true;
}
#pragma endregion

#pragma region Extra Functionalities
void *mhcalloc(size_t n, size_t size)
{
    if (n == 0)
        return NULL;
    void *ptr = mhalloc(n * size);
    if (ptr == NULL)
        return NULL;
    
    memset(ptr, 0, n * size);
    return (void *)ptr;
}

/* void *remhalloc(void *ptr, size_t newSize)
{

}*/
#pragma endregion