#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include <stdint.h>

typedef uint8_t Byte;

typedef struct ByteArray {
  uint16_t length;
  Byte* bytes;
} ByteArray;

typedef struct Utf8String {
  int32_t length;
  Byte* bytes;
} Utf8String;

typedef struct String {
  int32_t length;
  uint16_t* characters;
} String;

typedef struct Value {
  uint8_t is_pointer : 1;
  union {
    int32_t i32_v;
    int64_t i64_v;
    float f32_v;
    double f64_v;
    void* pointer;
  } u;
} Value;

typedef struct Function {
  Utf8String name;
  uint16_t locals;
  uint16_t stack;
  uint16_t args_size;
  ByteArray code;
} Function;

typedef struct FunctionArray {
  uint16_t count;
  Function* functions;
} FunctionArray;

typedef struct ValueArray {
  uint16_t count;
  Value* values;
} ValueArray;

enum TypeTag {
  TYPE_I32,
  TYPE_I64,
  TYPE_F32,
  TYPE_F64,
  TYPE_BOOL,
  TYPE_CHAR,
  TYPE_ARRAY
};

typedef struct TaggedValue {
  uint8_t tag : 1;
  union {
    int32_t i32_v;
    int64_t i64_v;
    float f32_v;
    double f64_v;
    void* pointer;
  } u;
} TaggedValue;

typedef struct ConstantPool {
  uint16_t count;
  TaggedValue* values;
} ConstantPool;

typedef struct Module {
  Utf8String name;
  ValueArray variables;
  FunctionArray functions;
} Module;

typedef struct Class {
  Utf8String name;
  ValueArray variables;
  FunctionArray functions;
} Class;

typedef struct ModuleArray {
  uint16_t count;
  Module* modules;
} ModuleArray;

typedef struct ClassArray {
  uint16_t count;
  Class* classes;
} ClassArray;

typedef struct Program {
  Utf8String name;
  ModuleArray modules;
  ClassArray classes;
  ConstantPool constantPool;
} Program;

typedef struct Array_I32 {
  int32_t length;
  int32_t* values;
} Array_I32;

typedef struct Array_I64 {
  int32_t length;
  int64_t* values;
} Array_I64;

typedef struct Array_F32 {
  int32_t length;
  float* values;
} Array_F32;

typedef struct Array_F64 {
  int32_t length;
  double* values;
} Array_F64;

typedef struct Array_Bool {
  int32_t length;
  uint8_t* values;
} Array_Bool;

typedef struct Array_Char {
  int32_t length;
  uint8_t* values;
} Array_Char;

typedef struct Array {
  uint8_t element_tag : 1;
  union {
    Array_I32 i32_array;
    Array_I64 i64_array;
    Array_F32 f32_array;
    Array_F64 f64_array;
    Array_Bool boolean_array;
    Array_Char char_array;
  } u;
} Array;

#endif /* DATA_STRUCTURES_H */