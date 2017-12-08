#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>

using std::wstring;
using byte = unsigned char;
using std::vector;

class Function
{
public:
	wstring name;
	int parametersSize;
	int frameSize;
	vector<byte>* code;
	Function(wstring name, int parametersSize, int frameSize, vector<byte>* code);
	~Function();
};

#endif // FUNCTION_H 
