#ifndef VM_MEMORY_H
#define VM_MEMORY_H
#include <inttypes.h>
#include <setjmp.h>
#include <stdio.h>

typedef uint8_t Byte;

typedef struct Block
{
	void* data;
	struct Block* next;
} Block;

typedef struct ByteCode
{
	int32_t size;
	int32_t offset;
	Byte* code;
}ByteCode;

typedef struct
{
	Block* blocks;
	Block* alloc_ptr;
	size_t mem_size;
	ByteCode byte_code;
	jmp_buf target;
} State;

State* state_new();

void* vm_alloc(State* state, size_t new_size);
void vm_free(State* state);
void vm_throw(State* state, int error_code);

#endif // VM_MEMORY_H