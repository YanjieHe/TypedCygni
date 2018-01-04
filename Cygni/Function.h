#ifndef FUNCTION_H
#define FUNCTION_H

#include <cstdint>
#include <string>
#include <vector>

using byte = unsigned char;
using i32 = int32_t;
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
