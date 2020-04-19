#ifndef MACHINE_H
#define MACHINE_H
#include <vector>
#include <string>
#include "OpCode.hpp"
#include "Structures.hpp"

class Machine {
public:
	std::vector<Value> stack;
	std::vector<Byte>* code;
	std::vector<Value>* constantPool;
	Function* function;
	Program* program;
	int fp;
	int sp;
	int pc;
	explicit Machine(int stackSize, Program* program);
	void Run();

	inline uint16_t USHORT(std::vector<Byte>* bytes, int index)
	{
		return static_cast<uint16_t>(((*bytes)[index]) + ((*bytes)[index + 1] << 8));
	}
};
#endif // MACHINE_H