#ifndef IO_H
#define IO_H
#include "Data.h"

#define GET_CHAR_ARRAY(VALUE) ((VALUE).u.gc_obj->obj->fields[0].u.gc_obj->arr)

extern "C" {
	__declspec(dllexport) int put_int(Value * env);
	__declspec(dllexport) int put_str(Value * env);
	__declspec(dllexport) int put_double(Value * env);
}

#endif // IO_H