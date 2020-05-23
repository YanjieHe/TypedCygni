#include "Memory.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

void * vm_alloc(State * state, size_t new_size)
{
	void* ptr;
	Block* new_block;

	ptr = malloc(new_size);

	if (ptr == NULL)
	{
		// handle the error
		fprintf(stderr, "fail to allocate memory\n");
		exit(-1);
	}
	else
	{
		new_block = malloc(sizeof(Block));
		new_block->data = ptr;
		new_block->next = NULL;
		if (state->alloc_ptr != NULL)
		{
			state->alloc_ptr->next = new_block;
			state->alloc_ptr = new_block;
		}
		else
		{
			state->alloc_ptr = new_block;
			state->blocks = new_block;
		}
		state->mem_size += new_size;
		return ptr;
	}
}

void vm_free(State * state)
{
	Block* node;
	Block* next;

	node = state->blocks;
	while (node)
	{
		free(node->data);
		next = node->next;
		free(node);
		node = next;
	}
}

void vm_throw(State * state, int error_code)
{
	long pos;

	vm_free(state);
	if (state->source)
	{
		pos = ftell(state->source);
		fprintf(stderr, "current byte code file position: %ld\n", pos);
		fclose(state->source);
		state->source = NULL;
	}
	longjmp(state->target, error_code);
}
