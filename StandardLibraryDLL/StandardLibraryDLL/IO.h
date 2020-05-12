#ifndef IO_H
#define IO_H
#include "Data.h"

extern "C" {
	__declspec(dllexport) void print_line_int(Value * argv, Value* out);
}

#endif // IO_H