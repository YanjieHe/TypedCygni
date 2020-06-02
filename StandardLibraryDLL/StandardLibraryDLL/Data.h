#ifndef VM_DATA_H
#define VM_DATA_H
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

typedef uint8_t Byte;
typedef uint32_t Char;

struct Object;
struct Function;

typedef struct
{
	uint8_t is_gc_obj : 1;
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

typedef enum
{
	ARRAY_TYPE_I32,
	ARRAY_TYPE_I64,
	ARRAY_TYPE_F32,
	ARRAY_TYPE_F64,
	ARRAY_TYPE_OBJECT
} ArrayType;

typedef struct
{
	ArrayType type;
	int32_t length;
	union
	{
		int32_t* i32_array;
		int64_t* i64_array;
		float_t* f32_array;
		double_t* f64_array;
		struct Object** obj_array;
	}u;
} Array;

typedef struct Object
{
	uint8_t marked : 1;
	uint8_t is_array;
	uint16_t class_index;
	union
	{
		Value* fields;
		Array* array;
	}u;
	struct Object* next;
}Object;

// unicode (UTF-32)
typedef struct
{
	int32_t length;
	Char* characters;
} String;

typedef struct
{
	uint8_t tag;
	union
	{
		int32_t i32_v;
		int64_t i64_v;
		float_t f32_v;
		double_t f64_v;
		String* str_v;
	}u;
} Constant;

typedef struct
{
	uint16_t need_stack_size;
	uint16_t args_size;
	uint16_t locals;
	uint16_t code_len;
	uint8_t* code;
	int n_constants;
	Constant* constant_pool;
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

typedef struct ConstantPool
{
	int32_t n_constants;
	Constant* constants;
} ConstantPool;

typedef struct
{
	char* name;
	uint16_t n_fields;
	char** field_names;
	uint16_t n_methods;
	Function** methods;
	ConstantPool constant_pool;
} ClassInfo;

typedef struct
{
	char* name;
	uint16_t n_fields;
	char** field_names;
	Value* variables;
	uint16_t n_functions;
	Function** functions;
	ConstantPool constant_pool;
} ModuleInfo;

typedef struct
{
	int size;
	int threshold;
	Object* first; /* first object */
} Heap;

typedef struct
{
	int class_count;
	ClassInfo* classes;
	int module_count;
	ModuleInfo* modules;
	Function* entry;
	Heap heap;
} Executable;

typedef enum VM_Error
{
	VM_ERROR_NO_ERROR = 0,
	VM_ERROR_READ_STRING,
	VM_ERROR_READ_U16,
	VM_ERROR_OPEN_FILE,
	VM_ERROR_READ_FUNCTION_BYTE_CODE,
	VM_ERROR_WRONG_TYPE_TAG,
	VM_ERROR_READ_TYPE_TAG,
} VM_Error;

#endif // VM_DATA_H