## Minimal memory allocator for learning purposes
* First-fit algorithm
* mhfree() uses metadata from mhalloc() so glibc free() won't work
* Reuses freed blocks if found
* Custom test.c file for debugging purposes

## To-do:
* Block coalescing
* Block splitting
* Implement mhcalloc() and mhrealloc()
