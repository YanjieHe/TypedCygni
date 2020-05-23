#ifndef IO_H
#define IO_H
#include "Data.h"

#define GET_CHAR_ARRAY(VALUE) ((VALUE).u.gc_obj->u.fields[0].u.gc_obj->u.array)

extern "C" {
	__declspec(dllexport) int put_int(Value * env);
	__declspec(dllexport) int put_str(Value * env);
}

#endif // IO_H