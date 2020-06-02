#ifndef VM_MACHINE_H
#define VM_MACHINE_H
#include "OpCode.h"
#include "Data.h"
#include "Memory.h"

typedef struct
{
	int stack_max_size;
	Value* stack;
	Function* function;
	Executable* exe;
	int fp;
	int sp;
	int pc;
	State* state;
} Machine;

static inline uint16_t bytes_to_u16(Byte* bytes, int index)
{
	return (((uint16_t)bytes[index]) << 8) + ((uint16_t)bytes[index + 1]);
}

Machine* create_machine(int stack_max_size, Executable* exe);

void run(Machine* machine);

void view_stack(Value* stack);

void copy_string(int32_t* char_array, String* str_v);

#endif // VM_MACHINE_H