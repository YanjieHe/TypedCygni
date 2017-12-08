#include "Function.h"

Function::Function(wstring name, int parametersSize, int frameSize, vector<byte>* code)
	:name{name}, parametersSize{parametersSize}, frameSize{frameSize}
{
	this->code = code;
}

Function::~Function()
{
	delete this->code;
}
