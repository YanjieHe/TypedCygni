#ifndef MACHINE_H
#define MACHINE_H
#include "OpCode.h"
#include "Data.h"

typedef struct
{
	int stack_max_size;
	Value* stack;
	Function* function;
	Executable* exe;
	int fp;
	int sp;
	int pc;
} Machine;

static inline uint16_t USHORT(Byte* bytes, int index)
{
	return ((uint16_t)bytes[index]) << 8 + ((uint16_t)bytes[index + 1]);
}

Machine* create_machine(int stack_max_size, Executable* exe);

void run(Machine* machine);

#endif // MACHINE_H