#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <cstdint>
#include <string>

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
	vector<byte>* code;
    Function(wstring name, i32 parameterSize, i32 frameSize, vector<byte>* code);
	~Function();
};

#endif // FUNCTION_H 
