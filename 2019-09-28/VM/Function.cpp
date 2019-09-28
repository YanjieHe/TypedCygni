#include "Function.h"
#include <iostream>

using namespace std;

Function::Function(wstring name, i32 parametersSize, i32 frameSize,
                   ByteCode code)
    : name{name},
      parametersSize{parametersSize}, frameSize{frameSize}, code{code}
{
}
