/* memory.c
   374, Memory Homework, 24AU
   Copyright N. Vitzthum
*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include "mem.h"  // outward facing functions
#include "mem_internal.h"  // private functions

// Default values for us to use later on
#define NODESIZE sizeof(freeNode)
#define MINCHUNK 16        // smallest allowable chunk of memory
#define BIGCHUNK 16000     // default of a very large chunk size

// Global variables for convenience
// these are static so outside code can't use them.
static freeNode* freeBlockList;  // points to list of available memory blocks
static uintptr_t totalMalloc;  // keeps track of memory allocated with malloc

/* The following functions need to be defined to meet the interface
   specified in mem.h.  These functions return or take the 'usable'
   memory addresses that a user would deal with.  They are called
   in the bench code.
*/

/* getmem returns the address of a usable block of memory that is
   at least size bytes large.  This code calls the helper function
   'get_block'
   Pre-condition: size is a positive integer
*/
void* getmem(uintptr_t size) {
  assert(size > 0);
  check_heap();
  // make sure size is a multiple of MINCHUNK (16):
  if (size % MINCHUNK != 0) {
    size = size + MINCHUNK -(size % MINCHUNK);
  }

  uintptr_t block = get_block(size);
  if (block == 0) {
    return NULL;
  }

  check_heap();

  return((void*)(block+NODESIZE));  // offset to get usable address
}

/* freemem uses the functions developed to add blocks to the
   list of available free blocks to return a node to the list.
   The pointer 'p' is the address of usable memory, allocated using getmem
*/
void freemem(void* p) {
  // COMPLETEME
  if (p == NULL) return;
  check_heap();

  // gets start of node address from memory address
  uintptr_t block = (uintptr_t)p - NODESIZE;

  return_block(block);
  check_heap();
}


uintptr_t get_block(uintptr_t size) {
  // COMPLETEME
  assert(size > 0);

  freeNode* prev = NULL;
  freeNode* curr = freeBlockList;
  // iterates through freeBlockList until a memory block is big
  // enough to return
  while (curr) {
    if (curr->size >= size) {
      if (curr->size > (size+MINCHUNK+NODESIZE)) {
          split_node(curr, size);
      }

      if (prev) {
        prev->next = curr->next;
      } else {
        freeBlockList = curr->next;
      }

      curr->next = NULL;
      return (uintptr_t)curr;
    }

    prev = curr;
    curr = curr->next;
  }

  // if no block was large enough or list was empty
  // a new block is created to be returned
  curr = new_block(size);
  if (prev) {
    prev->next = NULL;
  } else {
    freeBlockList = NULL;
  }

  curr->next = NULL;
  return (uintptr_t)curr;
}

freeNode* new_block(int size) {
  // COMPLETEME
  assert(size > 0);

  freeNode* newNode = NULL;

  if (size > BIGCHUNK) {
    newNode = (freeNode*)malloc(size+NODESIZE);
    newNode->size = size;
  } else {
    newNode = (freeNode*)malloc(BIGCHUNK+NODESIZE);
    newNode->size = BIGCHUNK;
  }

  if (!newNode) {
    fprintf(stderr, "Error: could not allocate memory\n");
    return NULL;
  }

  // puts block into the freeBlockList
  newNode->next = NULL;
  return_block((uintptr_t)newNode);

  return newNode;
}

void split_node(freeNode* n, uintptr_t size) {
  // COMPLETEME
  assert(size > 0);
  assert(n->size > (size + MINCHUNK + NODESIZE));

  // gets the address of where the remainder block of
  // memory should start from the inputted memory block address
  uintptr_t rem_addr = (uintptr_t)n + size + NODESIZE;
  freeNode* remainder = (freeNode*)rem_addr;

  // sets remainder's size and next values based on inputted
  // freeNode's values
  remainder->size = n->size - (size + NODESIZE);
  n->size = size;

  remainder->next = n->next;
  n->next = remainder;
}


void return_block(uintptr_t node) {
  // COMPLETEME
  freeNode* node1 = (freeNode*)node;
  freeNode* prev = NULL;
  freeNode* curr = freeBlockList;

  // iterates through freeBlockList until a node's address and size is larger
  // the address of the inputted address
  while (curr && (((uintptr_t)curr+NODESIZE+curr->size)<(uintptr_t)node1)) {
    prev = curr;
    curr = curr->next;
  }

  // inserts new node into the freeBlockList based on whether
  // it is inserted in front, middle, or back
  if (!curr && !prev) {
    freeBlockList = node1;
    freeBlockList->next = NULL;
    return;
  } else if (curr && !prev) {
    node1->next = curr;
    freeBlockList = node1;
  } else if (prev) {
    prev->next = node1;
    node1->next = curr;
  }

  // iterates through the freeBlockList and tests adjacency of
  // each node and combines it with the next node if adjacent
  freeNode* adj_test = freeBlockList;
  while (adj_test) {
    if (adjacent(adj_test)) {
      adj_test->size += adj_test->next->size + NODESIZE;
      adj_test->next = adj_test->next->next;
    } else {
      adj_test = adj_test->next;
    }
  }
}

int adjacent(freeNode* node) {
  // COMPLETEME
  if (!node || !node->next) {
    return 0;
  }

  // nodes are adjacent if the memory block of the first overlaps with the
  // address of the next block
  return ((uintptr_t)node + node->size + NODESIZE) >= (uintptr_t)(node->next);
}

/* The following are utility functions that may prove useful to you.
   They should work as presented, so you can leave them as is.
*/
void check_heap() {
  if (!freeBlockList) return;
  freeNode* currentNode = freeBlockList;
  uintptr_t minsize = currentNode->size;

  while (currentNode != NULL) {
    if (currentNode->size < minsize) {
      minsize = currentNode->size;
    }
    if (currentNode->next != NULL) {
      assert((uintptr_t)currentNode <(uintptr_t)(currentNode->next));
      assert((uintptr_t)currentNode + currentNode->size + NODESIZE
              <(uintptr_t)(currentNode->next));
    }
    currentNode = currentNode->next;
  }
  // go through free list and check for all the things
  if (minsize == 0) print_heap( stdout);
  assert(minsize >= MINCHUNK);
}

void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  *total_size = totalMalloc;
  *total_free = 0;
  *n_free_blocks = 0;

  freeNode* currentNode = freeBlockList;
  while (currentNode) {
    *n_free_blocks = *n_free_blocks + 1;
    *total_free = *total_free + (currentNode->size + NODESIZE);
    currentNode = currentNode->next;
  }
}

void print_heap(FILE *f) {
  printf("Printing the heap\n");
  freeNode* currentNode = freeBlockList;
  while (currentNode !=NULL) {
    fprintf(f, "%" PRIuPTR, (uintptr_t)currentNode);
    fprintf(f, ", size: %" PRIuPTR, currentNode->size);
    fprintf(f, ", next: %" PRIuPTR, (uintptr_t)currentNode->next);
    fprintf(f, "\n");
    currentNode = currentNode->next;
  }
}
