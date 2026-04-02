# Table of Contents
- [What is this?](#what-is-this)
- [Features](#features)
- [To-do](#to-do)
- [What I learned](#what-i-learned)
- [Last thoughts](#last-thoughts)

## What is this?
A custom memory allocator program for simulating `malloc()`, `free()`, `calloc()` and `realloc()` that I wrote in C for learning purposes.

## Features
- Uses `sbrk()` for requesting memory from heap.
- Minimal metadata consisting of 3 fields: size, status (free) and a pointer to the next block.
- Fully functional memory allocation and deallocation.
- Uses first-fit algorithm for less overhead, though will likely cause fragmentation problems later.
- Singly-linked list for linking memory blocks.
- Block splitting and coalescing (uses Doug Lea logic)

## To-do
- Add checks for invalid pointers.
- `remhalloc()`

## What I learned
- Metadata is present before every memory block, and not a universal metadata for all blocks.
- `free()` does not wipe memory, doing so would cause overhead. Instead, it messes with the block's status in the metadata.
- `sbrk()` increases the program's 'end' or program break. It essentially makes the space your program occupies larger, and uses that extra space from the heap for allocation.
- Doug Lea's concept suggests the use of a footer at the end of every free block, to store the size of the current block `block->size`. This is done for a faster O(1) lookup of the previous block during coalescing such that by traversing exactly `((char *)block - FOOTER_SIZE)` bytes, we can arrive at the previous block's footer. Now, we travel exactly `FOOTER_SIZE + prevBlockSize + METADATA_SIZE` bytes behind from the current block and arrive at the previous block's metadata.
- Additionally, we add `prevFree` at the struct to keep track of if the previous block is free or not.

## Last thoughts
Any lack in code performance and readability is purely due to my lack of understanding and experience. This project was made not for publication, but for the purpose of education, mostly my own. Permission is granted to anyone willing to use this repository for their own work.
