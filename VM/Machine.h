#ifndef MACHINE_H
#define MACHINE_H

#include "Any.h"
#include "Function.h"
#include <vector>
#include <string>

using std::vector;
using std::wstring;
using Heap = std::vector<Any>;
using ConstantPool = std::vector<Any>;

class Machine
{
public:
    Machine(ConstantPool pool, i32 staticSize);
	void Run(i32 entry);
	void LoadProgram(vector<byte>* code);
	void LoadFunction(Function f);

private:
	vector<byte>* code;
	vector<byte>* globalCode;

	i32 pc; // program counter
	i32 sp; // stack pointer
	i32 fp; // function pointer
	i32 ret; // return index
	bool running;

	Heap static_v;
	ConstantPool pool;
	Heap memory;

	vector<Function> functions;

	i32 ReadUShort();
	void MainLoop();
};

#endif // MACHINE_H 
