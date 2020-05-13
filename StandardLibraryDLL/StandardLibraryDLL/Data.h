#ifndef DATA_H
#define DATA_H
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

typedef uint8_t Byte;

struct Object;
struct Function;

typedef struct
{
	uint8_t is_gc_obj;
	union
	{
		int32_t i32_v;
		int64_t i64_v;
		float_t f32_v;
		double_t f64_v;
		struct Object* gc_obj; /* collectable objects */
		struct Function* function;
		void* pointer;
	}u;
} Value;

typedef struct Object
{
	struct Object* next;
	uint8_t marked;
	Value* fields;
	uint16_t class_index;
}Object;

typedef uint32_t Char;

// unicode (UTF32)
typedef struct
{
	int32_t length;
	Char* characters;
} String;

typedef struct
{
	uint16_t stack;
	uint16_t args_size;
	uint16_t locals;
	uint16_t code_len;
	uint8_t* code;
	int n_constants;
	Value* constant_pool;
} FunctionInfo;

typedef int(*FunctionPointer)(Value* env);

typedef struct NativeFunction
{
	bool is_loaded;
	char* lib_path;
	char* func_name;
	int args_size;
	FunctionPointer function_pointer;
} NativeFunction;

typedef struct Function
{
	bool is_native_function;
	char* name;
	union
	{
		FunctionInfo* func_info; // NULL if it is a native function
		NativeFunction* native_function;
	}u;
} Function;


typedef struct
{
	char* name;
	uint16_t n_fields;
	char** field_names;
	uint16_t n_methods;
	Function** methods;
	int n_constants;
	Value* constant_pool;
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
	Value* constant_pool;
}ModuleInfo;

typedef struct
{
	int class_count;
	ClassInfo* classes;
	int module_count;
	ModuleInfo* modules;
	Function* entry;
}Executable;

#endif // DATA_H