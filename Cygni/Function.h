#ifndef FUNCTION_H
#define FUNCTION_H

#include "PrimitiveType.h"
#include <string>
#include <vector>
using std::vector;
using std::wstring;

class Function
{
public:
	wstring name;
	i32 parameterSize;
	i32 frameSize;
    vector<byte> code;
    Function(wstring name, i32 parameterSize, i32 frameSize, vector<byte> code);
};

#endif // FUNCTION_H
