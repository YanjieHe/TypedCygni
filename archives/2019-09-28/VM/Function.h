#ifndef FUNCTION_H
#define FUNCTION_H

#include "Array"
#include "PrimitiveType.h"
#include <string>
#include <vector>
using std::vector;
using std::wstring;
using ByteCode = Array<byte>;

class Function
{
public:
    wstring name;
    i32 parametersSize;
    i32 frameSize;
    ByteCode code;
    Function(wstring name, i32 parametersSize, i32 frameSize, ByteCode code);
};
#endif // FUNCTION_H
