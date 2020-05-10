#ifndef DATA_H
#define DATA_H
#include <inttypes.h>
#include <math.h>
#include "OpCode.h"

typedef uint8_t Byte;

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


typedef uint32_t Char;

// unicode (UTF32)
typedef struct
{
	int32_t length;
	Char* characters;
} String;

typedef struct
{
	char* name;
	int n_parameters;
	int locals;
	int code_len;
	uint8_t* code;
	int n_constants;
	Value* constantPool;
} Function;



typedef struct
{
	char* name;
	uint16_t n_fields;
	char** field_names;
	uint16_t n_methods;
	Function** methods;
	int n_constants;
	Value* constantPool;
} ClassInfo;

typedef struct
{
	char* name;
	uint16_t n_fields;
	char** field_names;
	Value* variables;
	uint16_t n_functions;
	Function** functions;
	int n_constants;
	Value* constantPool;
}ModuleInfo;


typedef struct Object
{
	ClassInfo* class_info;
	Value* fields;
	struct Object* next;
}Object;

typedef struct
{
	int class_count;
	ClassInfo* classes;
	int module_count;
	ModuleInfo* modules;
	Function* entry;
}Executable;

#endif // DATA_H