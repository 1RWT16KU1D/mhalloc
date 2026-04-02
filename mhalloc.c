// Self-implementation of malloc(), free(), calloc(), realloc()
#include "mhalloc.h"
#include <stdbool.h>
#include <string.h> // For memset()
#include <unistd.h> // For sbrk()
#include <assert.h> // Will likely find a use for this later
#include <stdint.h> // For SIZE_MAX

// Metadata
typedef struct mhblock
{
    size_t size: 62;
    bool free: 1;
    bool prevFree: 1;
    struct mhblock *next;
} mhblock_t;

typedef struct // dlmalloc() implementation
{
    size_t size; // Replica of block->size after the main data
} mhblock_footer_t;

// Static to initialise values to 0
static mhblock_t *head = NULL;

#define METADATA_SIZE sizeof(mhblock_t)
#define FOOTER_SIZE sizeof(mhblock_footer_t)
#define MIN_BLOCK_SIZE (8 + FOOTER_SIZE)
#define ALIGN_SIZE(size, n) (((size) + ((n) - 1)) & ~((n) - 1))

#pragma region Helper Functions
// Could've used macros for these but meh
static inline bool isBlockUsable(mhblock_t *block, size_t size)
{
    return block->free && block->size >= size;
}

static inline bool isBlockSplittable(mhblock_t *block, size_t size)
{
    return block->size >= (size + METADATA_SIZE + MIN_BLOCK_SIZE);
}

static inline void initFooter(mhblock_t *block)
{
    mhblock_footer_t *footer = (mhblock_footer_t *)((char *)(block + 1) + block->size - FOOTER_SIZE);
    footer->size = block->size;
}

static mhblock_t *tryCoalesceBackward(mhblock_t *block)
{
    mhblock_footer_t *footer = (mhblock_footer_t *)((char *)block - FOOTER_SIZE);
    size_t prevBlockSize = footer->size;

    mhblock_t *prev = (mhblock_t *)((char *)block - prevBlockSize - METADATA_SIZE);
    prev->size += METADATA_SIZE + block->size;
    prev->next = block->next;
    initFooter(prev);

    if (prev->next)
        prev->next->prevFree = prev->free;
    return prev;
}

static void tryCoalesceForward(mhblock_t *block)
{
    mhblock_t *next = block->next;
    if (next && next->free)
    {
        block->next = next->next;
        block->size += METADATA_SIZE + next->size;
        initFooter(block);
    }
}
#pragma endregion

#pragma region Core Functions
void *mhalloc(size_t size)
{
    if (size == 0)
        return NULL;

    // Align size to 8 bytes
    size = ALIGN_SIZE(size, 8);

    // Initialise first block metadata
    if (head == NULL)
    {
        mhblock_t *ptr = (mhblock_t *)sbrk(METADATA_SIZE + size);
        if ((void *)ptr == (void *)-1)
            return NULL;

        head = ptr;

        head->size = size;
        head->free = false;
        head->prevFree = false;
        head->next = NULL;

        return (void *)(ptr + 1);
    }
    // Assign only suitable block
    mhblock_t *curr = head;
    mhblock_t *prev = NULL;

    while (curr)
    {
        if (isBlockUsable(curr, size))
        {
            curr->free = false;

            // Try split blocks into 2
            if (isBlockSplittable(curr, size))
            {
                mhblock_t *newBlock = (mhblock_t *)((char*)(curr + 1) + size); // Init pointer
                newBlock->size = curr->size - (size + METADATA_SIZE);
                newBlock->free = true;
                newBlock->next = curr->next;
                initFooter(newBlock);

                curr->size = size;
                curr->next = newBlock;
            }
            if (curr->next)
                curr->next->prevFree = false;
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
    ptr->prevFree = prev->free;
    ptr->next = NULL;

    return (void *)(ptr + 1);
}

void mhfree(void *ptr)
{
    // Free pointer only if valid
    if (ptr) 
    {
        mhblock_t *block = ((mhblock_t *)ptr - 1);
        block->free = true;
        initFooter(block);

        if (block->next)
            block->next->prevFree = true;

        if (block != head && block->prevFree)
            block = tryCoalesceBackward(block);
        if (block->next)
            tryCoalesceForward(block);
    }
}
#pragma endregion

#pragma region Extra Functionalities
void *mhcalloc(size_t n, size_t size)
{
    // Prevent division by zero and handle zero-size requests
    if (n == 0 || size == 0)
        return NULL;

    if (n > SIZE_MAX / size)
        return NULL;

    size_t totalSize = n * size;
    void *ptr = mhalloc(totalSize);
    
    if (ptr == NULL)
        return NULL;
    memset(ptr, 0, totalSize);
    return ptr;
}


/* void *remhalloc(void *ptr, size_t newSize)
{

}*/
#pragma endregion
