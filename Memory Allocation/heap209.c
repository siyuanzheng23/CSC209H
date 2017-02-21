/**
 * heap209.c
 * =========
 *
 * Implements a small heap space organized into chunks of free and allocated
 * space (maintained through linked lists.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>

#include "heap209.h"

void *heap_region = NULL;
Chunk *free_list = NULL;
Chunk *alloc_list = NULL;
Chunk *new_alloc;

/**
 * Allocate `nbytes` from the heap, and returns a pointer to the start of the
 * freshly allocated memory. Returns NULL if there is insufficient free
 * contiguous space.
 */
void *malloc209(size_t nbytes)
{
    
    void *newp;
    Chunk *prev,*f, *a,*temp1 = NULL,*temp2 = NULL, *temp3 = NULL;
    int leftnbytes;
    
    
    for (f = free_list, prev = NULL; f != NULL; prev = f ,f = f -> next){
        
        if(((f -> size) < nbytes)  && ((f -> next) == NULL)){
        /* allocation failed; take appropriate action. */
            return NULL;
        }
        else if ((f -> size) == nbytes){
            if (alloc_list == NULL){
                alloc_list = f;
                free_list = NULL;
                newp = alloc_list -> addr;
                return newp;
            }
            else if(alloc_list != NULL){
                for (a = alloc_list; a != NULL; a = a -> next){
                    if((a -> addr) < (f -> addr)){
                        temp1 = (Chunk *)malloc(sizeof(Chunk));
                        temp1 = a -> next;
                        prev -> next = f -> next;
                        a -> next = f;
                        f -> next = temp1;
                        free(temp1);
                        newp = (a -> next) -> addr;
                        return newp;
                    }
                }
            }
        
            return NULL;
        }
       
        else if ((f -> size) > nbytes){
            if (alloc_list == NULL){
                new_alloc = (Chunk *)malloc(sizeof(Chunk));
                alloc_list = new_alloc;
                alloc_list -> addr = (f -> addr);
                alloc_list -> size = nbytes;
                alloc_list -> next = NULL;
                leftnbytes = (f -> size) - nbytes;
                f -> size = leftnbytes;
                f -> addr =( f -> addr ) + nbytes;
                newp = alloc_list -> addr;
                return newp;
                
            }
            else if(alloc_list != NULL){
                for (a = alloc_list; a != NULL; a = a -> next){
                    if((a -> addr) < (f -> addr)){
                        temp1 = (Chunk *)malloc(sizeof(Chunk));
                        temp2 = (Chunk *)malloc(sizeof(Chunk));
                        temp3 = (Chunk *)malloc(sizeof(Chunk));
                        temp1 = a;
                        temp2 = a -> next;
                        leftnbytes = (f -> size) - nbytes;
                        temp3 -> addr = f -> addr;
                        temp3 -> size = nbytes;
                        temp3 -> next = temp2;
                        a -> next = temp3;
                        f -> size = leftnbytes;
                        f -> addr = (f -> addr) + nbytes;
                        newp = temp3 -> addr;
                        return newp;
                    }
                }
            }
            return NULL;
        }
    
    
    }
    return NULL;
}




    
    
    
    
    /* TODO FIXME: Implement this function. */
    

/**
 * Release the previously allocated memory pointed to by `addr` back into the
 * heap. Returns 0 on success, or -1 if `addr` was not found in the allocated
 * chunk list.
 */
int free209(void *addr)
{
    Chunk *prev,*f, *a, *temp1 = NULL, *prev2;
    /* TODO FIXME: Implement this function. */
    for(a = alloc_list, prev = NULL; a != NULL; prev = a,a = a -> next){
        if((addr != (a -> addr)) && ((a -> next) == NULL)){
            /* memory to be freed not previously allocated. exit the function. */
            return -1;
        }
        else if (addr == (a  -> addr)){
            for (f = free_list, prev2 = NULL; f != NULL; prev2 = f ,f = f -> next){
                if((f -> addr) < addr){
                    temp1 = (Chunk *)malloc(sizeof(Chunk));
                    temp1 -> addr = addr;
                    temp1 -> size = a -> size;
                    temp1 -> next = f -> next;
                    prev2 -> next = temp1;
                    temp1 -> next = f;
                    prev -> next = a -> next;
                    return 0;
                }
            }
        }
    }
    return -1;
}


/**
 * Initialize a large piece of contiguous memory to serve as the heap region.
 *
 * NB: mmap(1) is a system call that is used to create new regions within the
 * virtual (logical) address space of the calling process. It can be used to
 * map the contents of files into memory, so that you can directly access the
 * file data through memory pointers instead of requiring explicit read and
 * write function calls. Here we are using it in a slightly different way in
 * order to negotiate with the operating system to give us a large region of
 * memory for our private use (without any files being involved.)
 *
 * The `mmap` call will return a `void *` pointer to the allocated memory. It's
 * arguments are:
 *
 *  - NULL: let the kernel choose where to place the memory in the address
 *      space.
 *  - PROT_READ | PROT_WRITE: bitmask to indicate memory will be used for
 *      reading and writing
 *  - MAP_PRIVATE | MAP_ANON: create an anonymous mapping that is private (only
 *      visible) to this process
 *  - -1: no file descriptor as this mapping is not backed by a file
 *  - 0: no offest, again because no files are involved
 */

void heap209_init(size_t heap_size)
{
    
    
    if (heap_region) {
        fprintf(stderr, "ERROR: Cannot re-initialize the 209 heap\n");
        return;
    }

    heap_region = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

    if (heap_region == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }
    
    // TODO FIXME: Initialize the heap metadata lists
    else{
        free_list = (Chunk *)malloc(sizeof(Chunk));
        free_list -> addr = heap_region;
        free_list -> size = heap_size;
        free_list -> next = NULL;
        alloc_list = NULL;
    }
}

/**
 * Clean up all chunk metadata associated with the heap.
 */
void heap209_cleanup(void)
{
    Chunk *a, *f,*temp1 = NULL, *temp2 = NULL, *prev, *prev2;
    
     // TODO FIXME: Implement cleanup of all heap metadata
    
    for(f = free_list, prev = NULL; f != NULL; prev = f ,f = f -> next){
        if (f -> next == NULL){
            temp1 = (Chunk *)malloc(sizeof(Chunk));
            (temp1 -> addr) = (f -> addr);
        }
        if (prev != NULL){
            free(prev);
        }
    }
    free(temp1);
    
    
    for(a = alloc_list, prev2 = NULL; a != NULL; prev2 = a ,a = a -> next){
        if (a -> next == NULL){
            temp2 = (Chunk *)malloc(sizeof(Chunk));
            (temp2 -> addr) = (a -> addr);
        }
        if( prev2 != NULL){
        free(prev2);
        }
    }
    free(temp2);
}
