#include "Function.h"

Function::Function(i32 parametersSize, i32 frameSize, vector<byte>* code)
	:parametersSize{parametersSize}, frameSize{frameSize}, code{code}
{
}
