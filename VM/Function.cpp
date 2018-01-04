#include "Function.h"
#include <iostream>

using namespace std;

Function::Function(i32 parametersSize, i32 frameSize, ByteCode code)
    : parametersSize{parametersSize}, frameSize{frameSize}, code{code}
{
}

// i32 Print::Call(Memory* memory, i32 sp)
//{
//	i32 value = (*memory)[sp].i32_v;
//	wcout << value << endl;
//	return 1;
//}
