#include "Function.h"

Function::Function(wstring name, i32 parameterSize, i32 frameSize,
                   vector<byte> code)
    : name{name}, parameterSize{parameterSize}, frameSize{frameSize}, code{code}
{
}
