/* mem.c A template
 * My Name is
 * My User ID is
 * Lab4: Dynamic Memory Allocation
 * ECE 2230, Fall 2021
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "mem.h"

// Global variables required in mem.c only
// NEVER use DummyChunk in your allocation or free functions!!
static mem_chunk_t DummyChunk = {0, &DummyChunk};
static mem_chunk_t * Rover = &DummyChunk;   // one time initialization
int NumSbrkCalls = 0, NumPages = 0;


// private function prototypes
void mem_validate(void);

/* function to request 1 or more pages from the operating system.
 *
 * new_bytes must be the number of bytes that are being requested from
 *           the OS with the sbrk command.  It must be an integer 
 *           multiple of the PAGESIZE
 *
 * returns a pointer to the new memory location.  If the request for
 * new memory fails this function simply returns NULL, and assumes some
 * calling function will handle the error condition.  Since the error
 * condition is catastrophic, nothing can be done but to terminate 
 * the program.
 *
 * You can update this function to match your design.  But the method
 * to test sbrk much not be changed.  
 */
mem_chunk_t *morecore(int new_bytes) 
{
    char *cp;
    mem_chunk_t *new_p;
    // preconditions that must be true for all designs
    assert(new_bytes % PAGESIZE == 0 && new_bytes > 0);
    assert(PAGESIZE % sizeof(mem_chunk_t) == 0);
    cp = sbrk(new_bytes);
    if (cp == (char *) -1)  /* no space available */
        return NULL;
    new_p = (mem_chunk_t *) cp;
    
    NumSbrkCalls++; NumPages += new_bytes/PAGESIZE;
    return new_p;
}


// function to find the first available block of 
// sizeof(new_bytes)
void *find_first(int new_bytes)
{
	mem_chunk_t * start = Rover;

 	// loop through entire list
	while(Rover->next != start)
	{
		// first block of at least new_bytes
		if(Rover->size_units * sizeof(mem_chunk_t) >= new_bytes)
		{
			// set Rover back to starting position
			mem_chunk_t * match = Rover;
			Rover = start;
			return match;
		}
		Rover = Rover->next;
	}
	return NULL;
}

void *find_best(int new_bytes)
{
	mem_chunk_t * start = Rover;
	mem_chunk_t * best = NULL;

	// loop through entire list
	while(Rover->next != start)
	{
		// if block of at least new_bytes
		if(Rover->size_units * sizeof(mem_chunk_t) >= new_bytes)
		{
			if(best != NULL)
			{
				// if Rover is smaller than current best
				if(best->size_units > Rover->size_units)
					best = Rover;
			}
			else
			{
				// for first match when best is NULL
				best = Rover;
			}
		}
		Rover = Rover->next;
	}
	return best;
}

void *find_worst(int new_bytes)
{
	mem_chunk_t * start = Rover;
	mem_chunk_t * worst = NULL;
	Rover = Rover->next;

	// loop through entire list
	while(Rover->next != start)
	{
		// if block of at least new_bytes
		if(Rover->size_units * sizeof(mem_chunk_t) >= new_bytes)
		{
			if(worst != NULL)
			{
				// if Rover is larger than current worst, unless worst is an exact match
				if(worst->size_units < Rover->size_units && 
						worst->size_units != new_bytes / sizeof(mem_chunk_t))
					worst = Rover;
			}
			else
			{
				// for first match when worst is NULL
				worst = Rover;
			}
		}
		Rover = Rover->next;
	}
	return worst;
}

void coalesce()
{
	mem_chunk_t * start = Rover;
	unsigned long curr_addr, next_addr;

	// loop through entire list
	while(Rover->next != start)
	{
		curr_addr = (unsigned long) Rover;
		next_addr = (unsigned long) Rover->next;
		// if two concurrent free blocks
		if(curr_addr + sizeof(mem_chunk_t) + 
				Rover->size_units * sizeof(mem_chunk_t) == next_addr)
		{
			// combine blocks
			Rover->size_units += Rover->next->size_units + 1;
			Rover->next = Rover->next->next;
		}
		Rover = Rover->next;
	}
}
	
			
/* deallocates the space pointed to by return_ptr; it does nothing if
 * return_ptr is NULL.  
 *
 * This function assumes that the Rover pointer has already been 
 * initialized and points to some memory block in the free list.
 */
void Mem_free(void *return_ptr)
{
    // precondition
    assert(Rover != NULL && Rover->next != NULL);

    if(return_ptr == NULL) return;

    // assume p points to the start of the block to return to the free list
    // assert(p->size_units > 1);   // verify that the size field is valid
    // assert(p->next == NULL);    // this is not required but would be a good idea

    // obviously the next line is WRONG!!!!  You must fix it.
    free(return_ptr);
}

/* returns a pointer to space for an object of size nbytes, or NULL if the
 * request cannot be satisfied.  The memory is uninitialized.
 *
 * This function assumes that there is a Rover pointer that points to
 * some item in the free list.  
 */
void *Mem_alloc(const int nbytes)
{
    // precondition
    assert(nbytes > 0);
    assert(Rover != NULL && Rover->next != NULL);

    // allocates number of required units plus 1 for header
    // using bit manipulation to round to next page if needed
    int new_bytes = (nbytes + sizeof(mem_chunk_t) + (PAGESIZE - 1)) & ~ (PAGESIZE - 1);

    // assert((nunits-1)*sizeof(mem_chunk_t) >= nbytes);
    // assert((nunits-1)*sizeof(mem_chunk_t) < nbytes + sizeof(mem_chunk_t));

    // Insert your code here to find memory block

    // here are possible post-conditions, depending on your design. 
    // Include these tests before returning from this function.
    //
    // one unit is equal to sizeof(mem_chunk_t) bytes
    //
    // assume p is a pointer to the starting address of the memory block 
    // to be given to the user. The address returned to the user is q.
    // mem_chunk_t *p;
    // mem_chunk_t *q;
    //
    // assert(p + 1 == q);   // +1 means one unit or sizeof(mem_chunk_t)
    //
    // Next, test that the memory block is just big enough.
    // The minus one in the next two tests accounts for the header
    //
    // assert((p->size_units-1)*sizeof(mem_chunk_t) >= nbytes);
    // assert((p->size_units-1)*sizeof(mem_chunk_t) < nbytes + sizeof(mem_chunk_t));
    // assert(p->next == NULL);  // saftey first!
    // return q;


    // obviously the next line is WRONG!!!!  You must fix it.
    return malloc(nbytes);
}

/* prints stats about the current free list
 *
 * -- number of items in the linked list including dummy item
 * -- min, max, and average size of each item (in bytes)
 * -- total memory in list (in bytes)
 * -- number of calls to sbrk and number of pages requested
 *
 * A message is printed if all the memory is in the free list
 */
void Mem_stats(void)
{
    printf("delete me and implement mem stats\n");
    // One of the stats you must collect is the total number
    // of pages that have been requested using sbrk.
    // Say, you call this NumPages.  You also must count M,
    // the total number of bytes found in the free list 
    // (including all bytes used for headers).  If it is the case
    // that M == NumPages * PAGESiZE then print
    printf("all memory is in the heap -- no leaks are possible\n");
}

/* print table of memory in free list 
 *
 * The print should include the dummy item in the list 
 *
 * A unit is the size of one mem_chunk_t structure
 */
void Mem_print(void)
{
    // note position of Rover is not changed by this function
    assert(Rover != NULL && Rover->next != NULL);
    mem_chunk_t *p = Rover;
    mem_chunk_t *start = p;
    do {
        // example format.  Modify for your design
        printf("p=%p, size=%d (units), end=%p, next=%p %s\n", 
                p, p->size_units, p + p->size_units, p->next, 
                p->size_units!=0?"":"<-- dummy");
        p = p->next;
    } while (p != start);
    mem_validate();
}

/* This is an experimental function to attempt to validate the free
 * list when coalescing is used.  It is not clear that these tests
 * will be appropriate for all designs.  If your design utilizes a different
 * approach, that is fine.  You do not need to use this function and you
 * are not required to write your own validate function.
 */
void mem_validate(void)
{
    // note position of Rover is not changed by this function
    assert(Rover != NULL && Rover->next != NULL);
    assert(Rover->size_units >= 0);
    int wrapped = FALSE;
    int found_dummy = FALSE;
    int found_rover = FALSE;
    mem_chunk_t *p, *largest, *smallest;

    // for validate begin at DummyChunk
    p = &DummyChunk;
    do {
        if (p->size_units == 0) {
            assert(found_dummy == FALSE);
            found_dummy = TRUE;
        } else {
            assert(p->size_units > 0);
        }
        if (p == Rover) {
            assert(found_rover == FALSE);
            found_rover = TRUE;
        }
        p = p->next;
    } while (p != &DummyChunk);
    assert(found_dummy == TRUE);
    assert(found_rover == TRUE);

    if (Coalescing) {
        do {
            if (p >= p->next) {
                // this is not good unless at the one wrap around
                if (wrapped == TRUE) {
                    printf("validate: List is out of order, already found wrap\n");
                    printf("first largest %p, smallest %p\n", largest, smallest);
                    printf("second largest %p, smallest %p\n", p, p->next);
                    assert(0);   // stop and use gdb
                } else {
                    wrapped = TRUE;
                    largest = p;
                    smallest = p->next;
                }
            } else {
                assert(p + p->size_units < p->next);
            }
            p = p->next;
        } while (p != &DummyChunk);
        assert(wrapped == TRUE);
    }
}
/* vi:set ts=8 sts=4 sw=4 et: */

