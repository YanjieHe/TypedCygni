#ifndef VM_MEMORY_H
#define VM_MEMORY_H
#include <inttypes.h>
#include <setjmp.h>
#include <stdio.h>

typedef struct Block
{
	void* data;
	struct Block* next;
} Block;

typedef struct
{
	Block* blocks;
	Block* alloc_ptr;
	size_t mem_size;
	FILE* source; /* binary file */
	jmp_buf target;
} State;

void* vm_alloc(State* state, size_t new_size);
void vm_free(State* state);
void vm_throw(State* state, int error_code);
#endif // VM_MEMORY_H