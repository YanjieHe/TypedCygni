#ifndef IO_H
#define IO_H
#include "Data.h"

extern "C" {
	__declspec(dllexport) int print_line_int(Value * env);
}

#endif // IO_H