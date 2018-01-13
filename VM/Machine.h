#ifndef MACHINE_H
#define MACHINE_H

#include "Any.h"
#include "Array"
#include "Function.h"
#include <string>
#include <vector>

using std::vector;
using std::wstring;
using ByteCode = Array<byte>;
using Memory = Array<Any>;
using CodeStack = Array<ByteCode*>;
class Machine
{
public:
    Machine(Memory pool, i32 staticSize, ByteCode topCode,
            vector<Function> functions);
	void Run(i32 entry);
    void LoadFunction(Function f);

private:
    ByteCode& code;
    ByteCode topCode;
    CodeStack codeStack;
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

    i32 ReadUShort();
    Function& GetMainFunction();
    void MainLoop();
};

#endif // MACHINE_H
