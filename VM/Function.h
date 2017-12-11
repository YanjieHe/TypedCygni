#ifndef FUNCTION_H
#define FUNCTION_H

#include "Any.h"
#include <vector>

using byte = unsigned char;
using std::vector;

class Function
{
public:
	i32 parametersSize;
	i32 frameSize;
	vector<byte> code;
	Function(i32 parametersSize, i32 frameSize, vector<byte> code);
};

#endif // FUNCTION_H 
