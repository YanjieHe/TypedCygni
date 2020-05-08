#ifndef DATA_H
#define DATA_H

#include <cstdint>
#include <vector>
#include <string>
#include <inttypes.h>
#include "OpCode.hpp"

typedef uint8_t Byte;

typedef struct
{
	int length;
	int index;
	Byte* bytes;
}ByteCode;

typedef struct
{
	union
	{
		int32_t i32_v;
		int64_t i64_v;
		float_t f32_v;
		double_t f64_v;
		void* obj;
	}u;
	uint8_t is_pointer;
} Value;

typedef struct
{
	int length;
	Value* values;
}Values;


typedef struct
{
	char* name;
	int n_parameters;
	int locals;
	int code_len;
	uint8_t* code;
	Values constantPool;
} Function;

typedef struct
{
	int length;
	Function* values;
}Functions;

typedef struct Object
{
	Values fields;
	Functions* methods;
	struct Object* next;
}Object;

typedef struct
{
	char* name;
	uint16_t n_fields;
	char** field_names;
	Values constantPool;
	uint16_t n_methods;
	Function** methods;
}ClassInfo;

typedef struct
{
	char* name;
	uint16_t n_fields;
	char** field_names;
	Values variables;
	uint16_t n_functions;
	Function** functions;
	Values constantPool;
}ModuleInfo;

typedef struct
{
	int class_count;
	ClassInfo** classes;
	int module_count;
	ModuleInfo** modules;

	Object** singletons;
	Function* entry;
}Executable;

#endif // DATA_H