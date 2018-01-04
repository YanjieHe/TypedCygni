#ifndef MACHINE_H
#define MACHINE_H

#include "Any.h"
#include "ByteCode.h"
#include "Function.h"
#include "Memory.h"
#include <string>
#include <vector>

using std::vector;
using std::wstring;
using ByteCodeList = std::vector<ByteCode*>;

class Machine
{
public:
    Machine(Memory pool, i32 staticSize);
	void Run(i32 entry);
	void LoadProgram(ByteCode* code);
	void LoadFunction(Function f);

private:
	ByteCode* code;
	ByteCode* globalCode;
	ByteCodeList codeList;
    i32 codePointer;

    i32 pc;  // program counter
    i32 sp;  // stack pointer
    i32 fp;  // function pointer
    i32 ret; // return index
	bool running;

    Memory static_v;
    Memory pool;
	Memory memory;

	vector<Function> functions;

    u32 ReadUShort();
    i32 ReadUShortAsInt();
	void MainLoop();
};

#endif // MACHINE_H
