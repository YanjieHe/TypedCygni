#ifndef MACHINE_H
#define MACHINE_H

#include "Array"
#include "Decoder.h"
#include "Function.h"
#include "Value.h"
#include <string>
#include <vector>
using std::vector;
using std::wstring;
using ByteCode = Array<byte>;
using Memory = Array<Value>;

class Machine
{
public:
    Machine(i32 staticSize, Decoder& decoder);
	void Run(i32 entry);
    void LoadFunction(Function f);

private:
    ByteCode* code;
    Decoder& decoder;
    i32 codePointer = -1;

    i32 pc = 0;   // program counter
    i32 sp = -1;  // stack pointer
    i32 fp = 0;   // function pointer
    i32 ret = -1; // return index
    bool running = true;

    Memory static_v;
    Memory pool;
	Memory memory;

    i32 ReadUShort();
    i32 ReadUShortWithoutMoving();
    i32 GetMainFunction();
    void MainLoop();
};

#endif // MACHINE_H
