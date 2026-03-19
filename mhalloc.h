#pragma once

#include <stddef.h>

void *mhalloc(size_t size); // First-fit algorithm
void mhfree(void *ptr); // Uses mhalloc() block metadata
void *mhcalloc(size_t n, size_t size);
// void *remhalloc(void *ptr, size_t newSize);
