/**
 * @file   mm.c
 * @Author 02335 team
 * @date   September, 2024
 * @brief  Memory management skeleton.
 * 
 */

#include <stdint.h>

#include "mm.h"



/* Proposed data structure elements */

typedef struct header {
    struct header * next;     // Bit 0 is used to indicate free block
    uint64_t user_block[0];   // Standard trick: Empty array to make sure start of user block is aligned
} BlockHeader;

/* Macros to handle the free flag at bit 0 of the next pointer of header pointed at by p */
//we hide fri flag
#define GET_NEXT(p)    (void *) ((uintptr_t) (p->next) & ~0x1)   /* TODO: Mask out free flag */

//it means that p will point to n and n will keep the free flag unchanged
//if p->next is free, then n will be also free
#define SET_NEXT(p,n)  p->next = (void *)(((uintptr_t) (n)) | ((uintptr_t) p->next & 0x1)) /* TODO: Preserve free flag */

//will return 1, indicating that the block is free
#define GET_FREE(p)    (uint8_t) ( (uintptr_t) (p->next) & 0x1 )   /* OK -- do not change */

//Here we set the flag of the block to be free 1 or allocated 0
//f is the flag(should be 1 or 0). We do (f & ~0x1) to ensure that only the least bit of f is used when setting flag
//(p->next) & ~0x1 - we mask out the free bit of p->next to get the address/the same format no matter block is free or not
#define SET_FREE(p,f)  p->next = (void *) ( ((uintptr_t) (p->next) & ~0x1)|((uintptr_t)(f) & 0x1)) /* TODO: Set free bit of p->next to f */

//We find the size of the user data block by subtracting the address of the next block from the address of the current block
// and subtracting the size of the header.
#define SIZE(p)       (size_t) ((uintptr_t) GET_NEXT(p) - (uintptr_t) p - sizeof(BlockHeader)) /* TODO: Caluculate size of block from p and p->next */

#define MIN_SIZE     (8)   // A block should have at least 8 bytes available for the user


static BlockHeader * first = NULL;
static BlockHeader * current = NULL;

/**
 * @name    simple_init
 * @brief   Initialize the block structure within the available memory
 *
 */
void simple_init() {
    size_t alignment = 8;
    // We add alignment-1 (7 = 0111) to our start and end addresses to ensure that they will be able to be 8 byte aligned
    // We then use the and operation, to compare them with the negation of 7 (1000 = 8) and find the 8 byte aligned address they need
    uintptr_t aligned_memory_start = (((uintptr_t)memory_start + 7)/8)*8;  /* TODO: Alignment */
    uintptr_t aligned_memory_end   = (((uintptr_t)memory_end-1)/8)*8; /* TODO: Alignment */
    printf("Aligned memory start: %p, end: %p\n", (void*)aligned_memory_start, (void*)aligned_memory_end);
    BlockHeader * last;

    /* Already initialized ? */
    /* Already initialized ? */
    if (first == NULL) {
        /* Check that we have room for at least one free block and an end header*/
        if (aligned_memory_start + 2*sizeof(BlockHeader) + MIN_SIZE <= aligned_memory_end) {
            /* Initialize first block */
            //TODO: Place first and last blocks and set links and free flags properly
            first = (BlockHeader *)aligned_memory_start;
            last = (BlockHeader *)aligned_memory_end;
            SET_NEXT(first,last);
            SET_FREE(first,1);
            SET_NEXT(last, first);
            SET_FREE(last, 0);
        }
        current = first;
    }
}


/**
 * @name    simple_malloc
 * @brief   Allocate at least size contiguous bytes of memory and return a pointer to the first byte.
 *
 * This function should behave similar to a normal malloc implementation.
 *
 * @param size_t size Number of bytes to allocate.
 * @retval Pointer to the start of the allocated memory or NULL if not possible.
 *
 */
void* simple_malloc(size_t size) {

    if (first == NULL) {
        simple_init();
        if (first == NULL) return NULL;
    }

    size_t aligned_size = (size + (MIN_SIZE - 1)) / MIN_SIZE * MIN_SIZE;  /* TODO: Alignment */

    /* Search for a free block */
    BlockHeader * search_start = current;
    printf("Allocating block of size %lu bytes\n", aligned_size);
    printf("Current block address: %p\n", (void*)current);
    do {

        if (GET_FREE(current)) {

            /* Possibly coalesce consecutive free blocks here */


            /* Check if free block is large enough */
            if (SIZE(current) >= aligned_size) {
                /* Will the remainder be large enough for a new block? */
                if (SIZE(current) - aligned_size < sizeof(BlockHeader) + MIN_SIZE) {
                    /* TODO: Use block as is, marking it non-free*/
                    SET_FREE(current, 0);
                } else {
                    /* TODO: Carve aligned_size from block and allocate new free block for the rest */
                    //make a new block that will have the size of aligned_size
                    BlockHeader *new_block = (BlockHeader *) ((uintptr_t)current + sizeof(BlockHeader)+aligned_size);
                    // new block point to the next from curent
                    SET_NEXT(new_block, GET_NEXT(current));
                    SET_FREE(new_block, 1);
                    // current gets change to point to new_block
                    SET_NEXT(current, new_block);
                    SET_FREE(current, 0);
                }

                return (void *) current -> user_block; /* TODO: Return address of current's user_block and advance current */
            }
        }

        current = GET_NEXT(current);
    } while (current != search_start);

    /* None found */
    return NULL;

}


/**
 * @name    simple_free
 * @brief   Frees previously allocated memory and makes it available for subsequent calls to simple_malloc
 *
 * This function should behave similar to a normal free implementation.
 *
 * @param void *ptr Pointer to the memory to free.
 *
 */
void simple_free(void * ptr) {
    //
    BlockHeader * block = (void *)((uintptr_t)ptr - sizeof(BlockHeader)); /* TODO: Find block corresponding to ptr */
    if (GET_FREE(block)) {
        /* Block is not in use -- probably an error */
        return;
    }

    /* TODO: Free block */
    SET_FREE(block, 1);

    /* Possibly coalesce consecutive free blocks here */
}
#include "mm_aux.c"
