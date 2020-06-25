#ifndef VM_EXECUTABLE_H
#define VM_EXECUTABLE_H
#include "cygni.h"
#include "uthash.h"

typedef struct Constant {
  ConstantFlag flag;
  union {
    int32_t i32_v;
    int64_t i64_v;
    float_t f32_v;
    double_t f64_v;
    String *str_v;
    MethodRef *method_ref;
    ClassRef *class_ref;
    StaticVarRef *static_var_ref;
  };
} Constant;

typedef struct ConstantPool {
  u16 constant_pool_size;
  Constant *constants;
} ConstantPool;

typedef struct NativeMethod {
  bool is_loaded;
  char *lib_path;
  char *entry_point;
  FunctionPointer function_pointer;
} NativeMethod;

typedef struct MethodInfo {
  MethodFlag flag;
  struct ClassInfo *class_info;
  char *name;

  u16 need_stack_size;
  u16 args_size;
  u16 locals;
  u16 code_length;

  Byte *code;
  ConstantPool *constant_pool;
  NativeMethod native_method;
} MethodInfo;

typedef struct FieldInfo {
  char *name;
} FieldInfo;

typedef struct StaticVarInfo {
  char *name;
  Value value;
} StaticVarInfo;

typedef struct ClassInfo {
  ClassFlag flag;
  char *name;

  u16 fields_count;
  u16 methods_count;
  u16 static_vars_size;
  u16 static_funcs_size;
  u16 super_classes_count;
  u16 virtual_tables_count;
  u16 interfaces_count;

  FieldInfo *fields;
  MethodInfo *methods;
  StaticVarInfo *static_vars;
  MethodInfo *static_funcs;

  ConstantPool constant_pool;
  ClassRef *super_classes;
  ClassRef *interfaces;
  VirtualTable *virtual_tables;
} ClassInfo;

typedef struct {
  int size;
  int threshold;
  Object *first; /* first object */
} Heap;

typedef struct StaticFunctionMap {
  char *name;
  MethodInfo *function;
  UT_hash_handle hh;
} StaticFunctionMap;

typedef struct StaticVarMap {
  char *name;
  StaticVarInfo *static_var;
  UT_hash_handle hh;
} StaticVarMap;

typedef struct MethodMap {
  char *name;
  MethodInfo *method_info;
  UT_hash_handle hh;
} MethodMap;

typedef struct ClassMap {
  char *name;
  ClassInfo *class_info;
  UT_hash_handle hh;
} ClassMap;

typedef struct {
  i32 classes_count;
  MethodInfo *entry;
  StaticFunctionMap *static_function_map;
  StaticVarMap *static_var_map;
  ClassMap *class_map;
  MethodMap* method_map;
  ClassInfo *classes;
  Heap heap;
} Executable;
#endif // VM_EXECUTABLE_H