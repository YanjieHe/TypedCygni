#ifndef VM_DATA_H
#define VM_DATA_H
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>


typedef uint8_t Byte;
typedef uint32_t Char;

struct GCObject;
struct MethodInfo;
struct ClassInfo;

typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t i32;
typedef int64_t i64;

typedef enum ArrayType
{
	ARRAY_TYPE_I32,
	ARRAY_TYPE_I64,
	ARRAY_TYPE_F32,
	ARRAY_TYPE_F64,
	ARRAY_TYPE_OBJECT
} ArrayType;

typedef enum MethodFlag
{
	METHOD_FLAG_INSTANCE_METHOD,
	METHOD_FLAG_STATIC_METHOD,
	METHOD_FLAG_NATIVE_FUNCTION
}MethodFlag;

typedef enum ConstantFlag
{
	CONSTANT_FLAG_I32,
	CONSTANT_FLAG_I64,
	CONSTANT_FLAG_F32,
	CONSTANT_FLAG_F64,
	CONSTANT_FLAG_STRING,
	CONSTANT_FLAG_CLASS_NOT_LOADED,
	CONSTANT_FLAG_CLASS_LOADED,
	CONSTANT_FLAG_METHOD_NOT_LOADED,
	CONSTANT_FLAG_METHOD_LOADED,
} ConstantFlag;

typedef enum ClassFlag
{
	CLASS_FLAG_INTERFACE,
	CLASS_FLAG_CLASS
} ClassFlag;

typedef struct
{
	uint8_t is_gc_obj : 1;
	union
	{
		int32_t i32_v;
		int64_t i64_v;
		float_t f32_v;
		double_t f64_v;
		struct GCObject *gc_obj; /* collectable objects */
		struct MethodInfo *function;
		void *pointer;
	} u;
} Value;

typedef struct Object
{
	uint16_t type_id;
	Value *fields;
} Object;

typedef struct
{
	ArrayType type;
	int32_t length;
	union
	{
		int32_t *i32_array;
		int64_t *i64_array;
		float_t *f32_array;
		double_t *f64_array;
		struct GCObject **obj_array;
	} u;
} Array;

typedef struct GCObject
{
	uint8_t marked;
	uint8_t is_array;
	VirtualTable *v_table;
	struct GCObject* next;

	union
	{
		Object* obj;
		Array* arr;
	};
} GCObject;

typedef struct
{
	u16 classes_count;
	struct ClassInfo* classes;
	struct MethodInfo **methods;
} VirtualTable;

// unicode (UTF-32)
typedef struct
{
	int32_t length;
	Char *characters;
} String;

typedef struct MethodRef
{
	bool is_loaded;
	char* name;
	struct MethodInfo* method_info;
}MethodRef;

typedef struct ClassRef
{
	bool is_loaded;
	char* name;
	struct ClassInfo* class_info;
}ClassRef;

typedef struct
{
	ConstantFlag flag;
	union
	{
		int32_t i32_v;
		int64_t i64_v;
		float_t f32_v;
		double_t f64_v;
		String *str_v;
		MethodRef* method_ref;
		ClassRef* class_ref;
	};
} Constant;

typedef struct ConstantPool
{
	u16 constant_pool_size;
	Constant* constant_pool;
}ConstantPool;

typedef int(*FunctionPointer)(Value *env);

typedef struct NativeMethod
{
	bool is_loaded;
	char* lib_name;
	char* entry_point;
	FunctionPointer function_pointer;
} NativeMethod;

typedef struct MethodInfo
{
	struct ClassInfo* class_info;
	MethodFlag flag;
	char *name;
	u16 need_stack_size;
	u16 args_size;
	u16 locals;
	u16 code_length;
	u8 *code;
	ConstantPool* constant_pool;
	NativeMethod native_method;
} MethodInfo;

typedef struct
{
	char* name;
} FieldInfo;

typedef struct
{
	ClassFlag flag;
	char *name;

	u16 fields_count;
	FieldInfo* fields;

	ConstantPool* constant_pool;

	u16 static_size;
	Value* static_v;

	u16 methods_count;
	MethodInfo* methods;

	u16 super_classes_count;
	ClassInfo* super_classes;

	u16 interfaces_count;
	ClassInfo* interfaces;

	VirtualTable *virtual_tables;

} ClassInfo;

typedef struct
{
	int size;
	int threshold;
	Object *first; /* first object */
} Heap;

typedef struct
{
	i32 classes_count;
	ClassInfo *classes;
	MethodInfo *entry;
	Heap heap;
} Executable;

typedef enum VM_Error
{
	VM_ERROR_NO_ERROR = 0,
	VM_ERROR_READ_STRING,
	VM_ERROR_READ_U8,
	VM_ERROR_READ_U16,
	VM_ERROR_OPEN_FILE,
	VM_ERROR_READ_FUNCTION_BYTE_CODE,
	VM_ERROR_WRONG_TYPE_TAG,
	VM_ERROR_READ_TYPE_TAG,
	VM_ERROR_OPCODE,
	VM_ERROR_ARRAY_TYPE_CODE,
	VM_ERROR_LOAD_LIBRARY,
	VM_ERROR_LOAD_LIBRARY_FUNCTION,
} VM_Error;

#endif // VM_DATA_H