#ifndef MACHINE_H
#define MACHINE_H
#include <vector>
#include <string>
#include "OpCode.hpp"
#include "Data.h"

typedef Values Stack;

typedef struct
{
	Stack stack;
	Function* function;
	Executable* exe;
	int fp;
	int sp;
	int pc;
} Machine;

static inline uint16_t USHORT(Byte* bytes, int index)
{
	return (uint16_t)((bytes[index]) + (bytes[index + 1] << 8));
}

Machine* create_machine(int stackSize, Executable* exe);
void run(Machine* machine);

#endif // MACHINE_H