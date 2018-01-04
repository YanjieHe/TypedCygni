#ifndef FUNCTION_H
#define FUNCTION_H

#include "Any.h"
#include "ByteCode.h"
#include "Memory.h"
#include <vector>

using byte = unsigned char;
using std::vector;

class Function
{
public:
    i32 parametersSize;
    i32 frameSize;
	ByteCode code;
    i32 index;
    Function(i32 parametersSize, i32 frameSize, ByteCode code);
};

// class NativeFunction
//{
// public:
//	i32 parametersSize;
//	NativeFunction();
//	virtual i32 Call(Memory* memory, i32 sp) = 0;
//};

// class Print: public NativeFunction
//{
// public:
//	i32 Call(Memory* memory, i32 sp) override;
//};
#endif // FUNCTION_H
