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
struct VirtualTable;
struct StaticVarInfo;

typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t i32;
typedef int64_t i64;
typedef float_t f32;
typedef double_t f64;

typedef enum ArrayType {
  ARRAY_TYPE_I32,
  ARRAY_TYPE_I64,
  ARRAY_TYPE_F32,
  ARRAY_TYPE_F64,
  ARRAY_TYPE_OBJECT
} ArrayType;

typedef enum MethodFlag {
  METHOD_FLAG_NATIVE_FUNCTION = 0,
  METHOD_FLAG_INSTANCE_METHOD,
  METHOD_FLAG_MODULE_FUNCTION,
} MethodFlag;

typedef enum ConstantFlag {
  CONSTANT_FLAG_I32,
  CONSTANT_FLAG_I64,
  CONSTANT_FLAG_F32,
  CONSTANT_FLAG_F64,
  CONSTANT_FLAG_BOOLEAN,
  CONSTANT_FLAG_CHAR,
  CONSTANT_FLAG_STRING,
  CONSTANT_FLAG_METHOD,
  CONSTANT_FLAG_CLASS,
  CONSTANT_FLAG_STATIC_VAR,
  CONSTANT_FLAG_STATIC_FUNCTION,
} ConstantFlag;

typedef enum ClassFlag { CLASS_FLAG_INTERFACE, CLASS_FLAG_CLASS } ClassFlag;

typedef struct {
  uint8_t is_gc_obj : 1;
  union {
    int32_t i32_v;
    int64_t i64_v;
    float_t f32_v;
    double_t f64_v;
    struct GCObject *gc_obj; /* collectable objects */
    struct MethodInfo *function;
    void *pointer;
  } u;
} Value;

typedef struct Object {
  struct ClassInfo *type;
  Value *fields;
} Object;

typedef struct Array {
  ArrayType type;
  int32_t length;
  union {
    int32_t *i32_array;
    int64_t *i64_array;
    float_t *f32_array;
    double_t *f64_array;
    struct GCObject **obj_array;
  } u;
} Array;

typedef struct GCObject {
  u8 marked;
  u8 is_array;
  struct GCObject *next;

  union {
    Object *obj;
    Array *arr;
  };
} GCObject;

typedef struct MethodRef {
  char *name;
  struct MethodInfo *method_info;
} MethodRef;

typedef struct ClassRef {
  char *name;
  struct ClassInfo *class_info;
} ClassRef;

typedef struct StaticVarRef {
  char *name;
  struct StaticVarInfo *static_var;
} StaticVarRef;

typedef struct VirtualTable {
  ClassRef class_ref;
  u16 method_count;
  MethodRef *methods;
} VirtualTable;

// unicode (UTF-32)
typedef struct {
  int32_t length;
  Char *characters;
} String;

typedef int (*FunctionPointer)(Value *env);

typedef enum VM_Error {
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
  VM_ERROR_MISSING_MAIN_FUNCTION,
  VM_ERROR_STATIC_VAR_NOT_FOUND,
  VM_ERROR_STATIC_FUNCTION_NOT_FOUND,
  VM_ERROR_CLASS_NOT_FOUND,
  VM_ERROR_FAIL_TO_FIND_METHOD,
  VM_ERROR_FAIL_TO_FIND_CLASS_IN_VIRTUAL_TABLE
} VM_Error;

#endif // VM_DATA_H