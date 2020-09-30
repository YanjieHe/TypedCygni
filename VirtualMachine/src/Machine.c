#include "machine.h"
#include "library.h"
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define READ_U16(VALUE)                                                        \
  (VALUE) = (((uint16_t)code[pc]) << 8) + ((uint16_t)code[pc + 1]);            \
  pc = pc + 2;

#define BYTES_TO_U16(BYTES, OFFSET)                                            \
  ((((uint16_t)(BYTES)[(OFFSET)]) << 8) + ((uint16_t)(BYTES)[(OFFSET) + 1]))

#define STACK_PUSH_I32(VALUE)                                                  \
  sp++;                                                                        \
  stack[sp].u.i32_v = (VALUE);
#define STACK_PUSH_I64(VALUE)                                                  \
  sp++;                                                                        \
  stack[sp].u.i64_v = (VALUE);
#define STACK_PUSH_F32(VALUE)                                                  \
  sp++;                                                                        \
  stack[sp].u.f32_v = (VALUE);
#define STACK_PUSH_F64(VALUE)                                                  \
  sp++;                                                                        \
  stack[sp].u.f64_v = (VALUE);
#define STACK_PUSH_OBJECT(VALUE)                                               \
  sp++;                                                                        \
  stack[sp].u.gc_obj = (VALUE);                                                \
  stack[sp].is_gc_obj = true;
#define STACK_PUSH_FUNCTION(VALUE)                                             \
  sp++;                                                                        \
  stack[sp].u.function = (VALUE);

#define STACK_POP_I32(VALUE)                                                   \
  (VALUE) = stack[sp].u.i32_v;                                                 \
  sp--;
#define STACK_POP_I64(VALUE)                                                   \
  (VALUE) = stack[sp].u.i64_v;                                                 \
  sp--;
#define STACK_POP_F32(VALUE)                                                   \
  (VALUE) = stack[sp].u.f32_v;                                                 \
  sp--;
#define STACK_POP_F64(VALUE)                                                   \
  (VALUE) = stack[sp].u.f64_v;                                                 \
  sp--;
#define STACK_POP_OBJECT(VALUE)                                                \
  (VALUE) = stack[sp].u.gc_obj;                                                \
  stack[sp].is_gc_obj = false;                                                 \
  sp--;

#define ENSURE_CLASS_INITIALIZED(CLASS_INFO, CURRENT_PC)                       \
  if ((CLASS_INFO)->is_initialized == false) {                                 \
    int32_t stack_offset;                                                      \
    i32 cur_fp;                                                                \
    MethodInfo *next_func;                                                     \
                                                                               \
    (CLASS_INFO)->is_initialized = true;                                       \
    next_func = (CLASS_INFO)->initializer;                                     \
    cur_fp = fp;                                                               \
    fp = sp - next_func->args_size;                                            \
    stack_offset = fp + next_func->args_size + next_func->locals;              \
    stack[stack_offset].u.function = cur_func;                                 \
    stack[stack_offset + 1].u.i32_v = (CURRENT_PC);                            \
    stack[stack_offset + 2].u.i32_v = cur_fp;                                  \
    cur_func = next_func;                                                      \
    pc = 0;                                                                    \
    sp = stack_offset + 2;                                                     \
    code = next_func->code;                                                    \
    constant_pool = next_func->constant_pool->constants;                       \
    break;                                                                     \
  }

Machine *create_machine(int stack_max_size, Executable *exe) {
  Machine *machine;
  int i;

  machine = malloc(sizeof(Machine));
  machine->stack_max_size = stack_max_size;
  machine->stack = malloc(sizeof(Value) * stack_max_size);
  machine->exe = exe;
  machine->fp = 0;
  machine->sp = -1;
  machine->pc = 0;
  machine->function = NULL;

  for (i = 0; i < stack_max_size; i++) {
    machine->stack[i].is_gc_obj = false;
  }
  return machine;
}

void run(Machine *machine) {
  Byte *code;
  Constant *constant_pool;
  Value *stack;
  int pc;
  int fp;
  int sp;
  Byte op;
  MethodInfo *prev_func;
  MethodInfo *cur_func;

  machine->function = machine->exe->entry;
  cur_func = machine->function;
  code = machine->function->code;
  constant_pool = machine->function->constant_pool->constants;
  stack = machine->stack;

  entry(machine);
  pc = 0;
  fp = machine->fp;
  sp = machine->sp;

  while (pc < cur_func->code_length) {
    op = code[pc];
    // view_stack(machine, stack, sp, fp);
    // printf("function: %s, op = %s, sp = %d, fp = %d, pc = %d\n",
    // cur_func->name,
    //       opcode_info[op][0], sp, fp, pc);
    pc = pc + 1;
    switch (op) {
    case PUSH_I32_0: {
      sp++;
      stack[sp].u.i32_v = 0;
      break;
    }
    case PUSH_I32_1: {
      sp++;
      stack[sp].u.i32_v = 1;
      break;
    }
    case PUSH_I32: {
      int32_t const_i32;
      u16 constant_pool_offset;

      READ_U16(constant_pool_offset);
      const_i32 = constant_pool[constant_pool_offset].i32_v;
      STACK_PUSH_I32(const_i32);
      break;
    }
    case PUSH_I64: {
      int64_t const_i64;
      u16 constant_pool_offset;

      READ_U16(constant_pool_offset);
      const_i64 = constant_pool[constant_pool_offset].i64_v;
      STACK_PUSH_I64(const_i64);
      break;
    }
    case PUSH_F32: {
      float_t const_f32;
      u16 constant_pool_offset;

      READ_U16(constant_pool_offset);
      const_f32 = constant_pool[constant_pool_offset].f32_v;
      STACK_PUSH_F32(const_f32);
      break;
    }
    case PUSH_F64: {
      double_t const_f64;
      u16 constant_pool_offset;

      READ_U16(constant_pool_offset);
      const_f64 = constant_pool[constant_pool_offset].f64_v;
      STACK_PUSH_F64(const_f64);
      break;
    }
    case PUSH_STRING: {
      GCObject *str_obj;
      String *const_str;
      u16 constant_pool_offset;

      READ_U16(constant_pool_offset);
      const_str = constant_pool[constant_pool_offset].str_v;

      str_obj = malloc(sizeof(GCObject));
      str_obj->is_array = true;
      str_obj->next = NULL;
      str_obj->arr = malloc(sizeof(Array));
      str_obj->arr->length = const_str->length;
      str_obj->arr->type = ARRAY_TYPE_I32;
      str_obj->arr->u.i32_array = malloc(sizeof(int32_t) * const_str->length);
      copy_string(str_obj->arr->u.i32_array, const_str);

      STACK_PUSH_OBJECT(str_obj);
      break;
    }
    case PUSH_LOCAL_I32: {
      uint16_t offset;
      int32_t local_i32;

      READ_U16(offset);
      local_i32 = stack[fp + offset].u.i32_v;
      STACK_PUSH_I32(local_i32);
      break;
    }
    case PUSH_LOCAL_I64: {
      uint16_t offset;
      int64_t local_i64;

      READ_U16(offset);
      local_i64 = stack[fp + offset].u.i64_v;
      STACK_PUSH_I64(local_i64);
      break;
    }
    case PUSH_LOCAL_F32: {
      uint16_t offset;
      float_t local_f32;

      READ_U16(offset);
      local_f32 = stack[fp + offset].u.f32_v;
      STACK_PUSH_F32(local_f32);
      break;
    }
    case PUSH_LOCAL_F64: {
      uint16_t offset;
      double_t local_f64;

      READ_U16(offset);
      local_f64 = stack[fp + offset].u.f64_v;
      STACK_PUSH_F64(local_f64);
      break;
    }
    case PUSH_LOCAL_OBJECT: {
      uint16_t offset;
      GCObject *local_obj;

      READ_U16(offset);
      // printf("fp = %d, offset = %d\n", fp, offset);
      local_obj = stack[fp + offset].u.gc_obj;
      STACK_PUSH_OBJECT(local_obj);
      break;
    }
    case POP_LOCAL_I32: {
      uint16_t offset;
      int32_t local_i32;

      READ_U16(offset);
      STACK_POP_I32(local_i32);
      stack[fp + offset].u.i32_v = local_i32;
      break;
    }
    case POP_LOCAL_I64: {
      uint16_t offset;
      int64_t local_i64;

      READ_U16(offset);
      STACK_POP_I64(local_i64);
      stack[fp + offset].u.i64_v = local_i64;
      break;
    }
    case POP_LOCAL_F32: {
      uint16_t offset;
      float_t local_f32;

      READ_U16(offset);
      STACK_POP_F32(local_f32);
      stack[fp + offset].u.f32_v = local_f32;
      break;
    }
    case POP_LOCAL_F64: {
      uint16_t offset;
      double_t local_f64;

      READ_U16(offset);
      STACK_POP_F64(local_f64);
      stack[fp + offset].u.f64_v = local_f64;
      break;
    }
    case POP_LOCAL_OBJECT: {
      uint16_t offset;
      GCObject *local_obj;

      READ_U16(offset);
      STACK_POP_OBJECT(local_obj);
      stack[fp + offset].u.gc_obj = local_obj;
      stack[fp + offset].is_gc_obj = true;
      break;
    }
    case PUSH_STATIC_I32: {
      u16 constant_pool_offset;
      i32 static_i32;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      ENSURE_CLASS_INITIALIZED(static_var->class_info, pc - 3);
      static_i32 = static_var->value.u.i32_v;
      STACK_PUSH_I32(static_i32);
      break;
    }
    case PUSH_STATIC_I64: {
      u16 constant_pool_offset;
      i64 static_i64;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      ENSURE_CLASS_INITIALIZED(static_var->class_info, pc - 3);
      static_i64 = static_var->value.u.i64_v;
      STACK_PUSH_I64(static_i64);
      break;
    }
    case PUSH_STATIC_F32: {
      u16 constant_pool_offset;
      f32 static_f32;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      ENSURE_CLASS_INITIALIZED(static_var->class_info, pc - 3);
      static_f32 = static_var->value.u.f32_v;
      STACK_PUSH_F32(static_f32);
      break;
    }
    case PUSH_STATIC_F64: {
      u16 constant_pool_offset;
      f64 static_f64;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      ENSURE_CLASS_INITIALIZED(static_var->class_info, pc - 3);
      static_f64 = static_var->value.u.f64_v;
      STACK_PUSH_F64(static_f64);
      break;
    }
    case PUSH_STATIC_OBJECT: {
      u16 constant_pool_offset;
      GCObject *static_obj;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      ENSURE_CLASS_INITIALIZED(static_var->class_info, pc - 3);
      static_obj = static_var->value.u.gc_obj;
      STACK_PUSH_OBJECT(static_obj);
      break;
    }
    case POP_STATIC_I32: {
      u16 constant_pool_offset;
      i32 static_i32;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      STACK_POP_I32(static_i32);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      static_var->value.u.i32_v = static_i32;
      break;
    }
    case POP_STATIC_I64: {
      u16 constant_pool_offset;
      i64 static_i64;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      STACK_POP_I64(static_i64);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      static_var->value.u.i64_v = static_i64;
      break;
    }
    case POP_STATIC_F32: {
      u16 constant_pool_offset;
      f32 static_f32;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      STACK_POP_F32(static_f32);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      static_var->value.u.f32_v = static_f32;
      break;
    }
    case POP_STATIC_F64: {
      u16 constant_pool_offset;
      f64 static_f64;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      STACK_POP_F64(static_f64);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      static_var->value.u.f64_v = static_f64;
      break;
    }
    case POP_STATIC_OBJECT: {
      u16 constant_pool_offset;
      GCObject *static_obj;
      StaticVarInfo *static_var;

      READ_U16(constant_pool_offset);
      STACK_POP_OBJECT(static_obj);
      static_var = get_static_var(
          machine, constant_pool[constant_pool_offset].static_var_ref);
      static_var->value.u.gc_obj = static_obj;
      break;
    }
    case ADD_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v + stack[sp].u.i32_v);
      sp--;
      break;
    }
    case ADD_I64: {
      stack[sp - 1].u.i64_v = (stack[sp - 1].u.i64_v + stack[sp].u.i64_v);
      sp--;
      break;
    }
    case ADD_F32: {
      stack[sp - 1].u.f32_v = (stack[sp - 1].u.f32_v + stack[sp].u.f32_v);
      sp--;
      break;
    }
    case ADD_F64: {
      stack[sp - 1].u.f64_v = (stack[sp - 1].u.f64_v + stack[sp].u.f64_v);
      sp--;
      break;
    }
    case SUB_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v - stack[sp].u.i32_v);
      sp--;
      break;
    }
    case SUB_I64: {
      stack[sp - 1].u.i64_v = (stack[sp - 1].u.i64_v - stack[sp].u.i64_v);
      sp--;
      break;
    }
    case SUB_F32: {
      stack[sp - 1].u.f32_v = (stack[sp - 1].u.f32_v - stack[sp].u.f32_v);
      sp--;
      break;
    }
    case SUB_F64: {
      stack[sp - 1].u.f64_v = (stack[sp - 1].u.f64_v - stack[sp].u.f64_v);
      sp--;
      break;
    }
    case MUL_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v * stack[sp].u.i32_v);
      sp--;
      break;
    }
    case MUL_I64: {
      stack[sp - 1].u.i64_v = (stack[sp - 1].u.i64_v * stack[sp].u.i64_v);
      sp--;
      break;
    }
    case MUL_F32: {
      stack[sp - 1].u.f32_v = (stack[sp - 1].u.f32_v * stack[sp].u.f32_v);
      sp--;
      break;
    }
    case MUL_F64: {
      stack[sp - 1].u.f64_v = (stack[sp - 1].u.f64_v * stack[sp].u.f64_v);
      sp--;
      break;
    }
    case DIV_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v / stack[sp].u.i32_v);
      sp--;
      break;
    }
    case DIV_I64: {
      stack[sp - 1].u.i64_v = (stack[sp - 1].u.i64_v / stack[sp].u.i64_v);
      sp--;
      break;
    }
    case DIV_F32: {
      stack[sp - 1].u.f32_v = (stack[sp - 1].u.f32_v / stack[sp].u.f32_v);
      sp--;
      break;
    }
    case DIV_F64: {
      stack[sp - 1].u.f64_v = (stack[sp - 1].u.f64_v / stack[sp].u.f64_v);
      sp--;
      break;
    }
    case GT_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v > stack[sp].u.i32_v);
      sp--;
      break;
    }
    case GT_I64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v > stack[sp].u.i64_v);
      sp--;
      break;
    }
    case GT_F32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v > stack[sp].u.f32_v);
      sp--;
      break;
    }
    case GT_F64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v > stack[sp].u.f64_v);
      sp--;
      break;
    }
    case LT_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v < stack[sp].u.i32_v);
      sp--;
      break;
    }
    case LT_I64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v < stack[sp].u.i64_v);
      sp--;
      break;
    }
    case LT_F32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v < stack[sp].u.f32_v);
      sp--;
      break;
    }
    case LT_F64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v < stack[sp].u.f64_v);
      sp--;
      break;
    }
    case GE_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v >= stack[sp].u.i32_v);
      sp--;
      break;
    }
    case GE_I64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v >= stack[sp].u.i64_v);
      sp--;
      break;
    }
    case GE_F32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v >= stack[sp].u.f32_v);
      sp--;
      break;
    }
    case GE_F64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v >= stack[sp].u.f64_v);
      sp--;
      break;
    }
    case LE_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v <= stack[sp].u.i32_v);
      sp--;
      break;
    }
    case LE_I64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v <= stack[sp].u.i64_v);
      sp--;
      break;
    }
    case LE_F32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v <= stack[sp].u.f32_v);
      sp--;
      break;
    }
    case LE_F64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v <= stack[sp].u.f64_v);
      sp--;
      break;
    }
    case EQ_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v == stack[sp].u.i32_v);
      sp--;
      break;
    }
    case EQ_I64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v == stack[sp].u.i64_v);
      sp--;
      break;
    }
    case EQ_F32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v == stack[sp].u.f32_v);
      sp--;
      break;
    }
    case EQ_F64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v == stack[sp].u.f64_v);
      sp--;
      break;
    }
    case NE_I32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v != stack[sp].u.i32_v);
      sp--;
      break;
    }
    case NE_I64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v != stack[sp].u.i64_v);
      sp--;
      break;
    }
    case NE_F32: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v != stack[sp].u.f32_v);
      sp--;
      break;
    }
    case NE_F64: {
      stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v != stack[sp].u.f64_v);
      sp--;
      break;
    }
    case PUSH_FIELD_I32: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp].u.gc_obj;
      READ_U16(offset);
      // printf("PUSH_FIELD_I32\n");
      // printf("is gc obj: %d\n", stack[sp].is_gc_obj);
      // printf("offset = %d\n", offset);
      // view_stack(stack, sp);
      // printf("class index = %d\n", gc_obj->class_index);
      // printf("field name: %s\n",
      // machine->exe->classes[gc_obj->class_index].field_names[offset]);
      // printf("field int: %d\n", gc_obj->u.fields[offset].u.i32_v);
      stack[sp].u.i32_v = instance->obj->fields[offset].u.i32_v;
      stack[sp].is_gc_obj = false;
      break;
    }
    case PUSH_FIELD_I64: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp].u.gc_obj;
      READ_U16(offset);
      stack[sp].u.i64_v = instance->obj->fields[offset].u.i64_v;
      stack[sp].is_gc_obj = false;
      break;
    }
    case PUSH_FIELD_F32: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp].u.gc_obj;
      READ_U16(offset);
      stack[sp].u.f32_v = instance->obj->fields[offset].u.f32_v;
      stack[sp].is_gc_obj = false;
      break;
    }
    case PUSH_FIELD_F64: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp].u.gc_obj;
      READ_U16(offset);
      stack[sp].u.f64_v = instance->obj->fields[offset].u.f64_v;
      stack[sp].is_gc_obj = false;
      break;
    }
    case PUSH_FIELD_OBJECT: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp].u.gc_obj;
      READ_U16(offset);
      // printf("rect address %ld\n", (long)gc_obj);
      // printf("address %ld\n", (long)gc_obj->u.fields[offset].u.gc_obj);
      stack[sp].u.gc_obj = instance->obj->fields[offset].u.gc_obj;
      stack[sp].is_gc_obj = true;
      // printf("address %ld\n", (long)stack[sp].u.gc_obj);
      break;
    }
    case POP_FIELD_I32: {
      uint16_t offset;
      GCObject *instance;

      // printf("is gc obj? %d\n", stack[sp - 1].is_gc_obj);
      instance = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_U16(offset);
      // printf("offset = %d\n", offset);
      instance->obj->fields[offset].u.i32_v = stack[sp].u.i32_v;
      // printf("POP_FIELD_I32 gc_obj->u.fields[offset].u.i32 = %d\n",
      // gc_obj->u.fields[offset].u.i32_v);
      sp = sp - 2;
      break;
    }
    case POP_FIELD_I64: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_U16(offset);
      instance->obj->fields[offset].u.i64_v = stack[sp].u.i64_v;
      sp = sp - 2;
      break;
    }
    case POP_FIELD_F32: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_U16(offset);
      instance->obj->fields[offset].u.f32_v = stack[sp].u.f32_v;
      sp = sp - 2;
      break;
    }
    case POP_FIELD_F64: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_U16(offset);
      instance->obj->fields[offset].u.f64_v = stack[sp].u.f64_v;
      sp = sp - 2;
      break;
    }
    case POP_FIELD_OBJECT: {
      uint16_t offset;
      GCObject *instance;

      instance = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_U16(offset);
      // printf("offset = %d\n", offset);
      // printf("class index = %d\n", gc_obj->class_index);
      // printf("field name: %s\n",
      // machine->exe->classes[gc_obj->class_index].field_names[offset]);
      // printf("field string: is array? %d\n",
      // stack[sp].u.gc_obj->u.fields[0].u.gc_obj->is_array); printf("rect
      // address %ld\n", (long)stack[sp - 1].u.gc_obj); printf("rect address
      // %ld\n", (long)gc_obj); printf("address %ld\n",
      // (long)stack[sp].u.gc_obj);
      instance->obj->fields[offset].u.gc_obj = stack[sp].u.gc_obj;
      stack[sp].is_gc_obj = false;
      instance->obj->fields[offset].is_gc_obj = true;
      // printf("address %ld\n", (long)gc_obj->u.fields[offset].u.gc_obj);
      // printf("field 'name' class %s\n",
      //	machine->exe->classes[gc_obj->u.fields[offset].u.gc_obj->class_index].name);
      sp = sp - 2;
      break;
    }
    case PUSH_ARRAY_I32: {
      GCObject *array;
      int32_t index;
      int32_t item;

      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      item = array->arr->u.i32_array[index];
      STACK_PUSH_I32(item);
      break;
    }
    case PUSH_ARRAY_I64: {
      GCObject *array;
      int32_t index;
      int64_t item;

      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      item = array->arr->u.i64_array[index];
      STACK_PUSH_I64(item);
      break;
    }
    case PUSH_ARRAY_F32: {
      GCObject *array;
      int32_t index;
      float_t item;

      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      item = array->arr->u.f32_array[index];
      STACK_PUSH_F32(item);
      break;
    }
    case PUSH_ARRAY_F64: {
      GCObject *array;
      int32_t index;
      double_t item;

      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      item = array->arr->u.f64_array[index];
      STACK_PUSH_F64(item);
      break;
    }
    case PUSH_ARRAY_OBJECT: {
      GCObject *array;
      int32_t index;
      GCObject *item;

      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      item = array->arr->u.obj_array[index];
      STACK_PUSH_OBJECT(item);
      break;
    }
    case POP_ARRAY_I32: {
      GCObject *array;
      int32_t index;
      int32_t item;

      STACK_POP_I32(item);
      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      array->arr->u.i32_array[index] = item;
      break;
    }
    case POP_ARRAY_I64: {
      GCObject *array;
      int32_t index;
      int64_t item;

      STACK_POP_I64(item);
      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      array->arr->u.i64_array[index] = item;
      break;
    }
    case POP_ARRAY_F32: {
      GCObject *array;
      int32_t index;
      float_t item;

      STACK_POP_F32(item);
      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      array->arr->u.f32_array[index] = item;
      break;
    }
    case POP_ARRAY_F64: {
      GCObject *array;
      int32_t index;
      double_t item;

      STACK_POP_F64(item);
      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      array->arr->u.f64_array[index] = item;
      break;
    }
    case POP_ARRAY_OBJECT: {

      GCObject *array;
      int32_t index;
      GCObject *item;

      STACK_POP_OBJECT(item);
      STACK_POP_I32(index);
      STACK_POP_OBJECT(array);
      array->arr->u.obj_array[index] = item;
      break;
    }
    case JUMP: {
      pc = BYTES_TO_U16(code, pc);
      break;
    }
    case JUMP_IF_TRUE: {
      uint32_t bool_val;

      STACK_POP_I32(bool_val);
      if (bool_val) {
        pc = BYTES_TO_U16(code, pc);
      } else {
        pc = pc + 2;
      }
      break;
    }
    case JUMP_IF_FALSE: {
      uint32_t bool_val;

      STACK_POP_I32(bool_val);
      if (bool_val) {
        pc = pc + 2;
      } else {
        pc = BYTES_TO_U16(code, pc);
      }
      break;
    }
    case RETURN_I32: {
      int32_t result_i32;
      int32_t stack_offset;

      STACK_POP_I32(result_i32);
      stack_offset = fp + cur_func->args_size + cur_func->locals;
      prev_func = stack[stack_offset].u.function;
      if (prev_func == NULL) {
        // exit from the main function
        printf("EXIT FROM THE MAIN FUNCTION!\n");
        return;
      }
      sp = fp;
      pc = stack[stack_offset + 1].u.i32_v;
      fp = stack[stack_offset + 2].u.i32_v;
      code = prev_func->code;
      constant_pool = prev_func->constant_pool->constants;
      cur_func = prev_func;

      stack[sp].u.i32_v = result_i32;

      break;
    }
    case RETURN_I64: {
      int64_t result_i64;
      int32_t stack_offset;

      STACK_POP_I64(result_i64);
      stack_offset = fp + cur_func->args_size + cur_func->locals;
      prev_func = stack[stack_offset].u.function;
      sp = fp;
      pc = stack[stack_offset + 1].u.i32_v;
      fp = stack[stack_offset + 2].u.i32_v;
      code = prev_func->code;
      constant_pool = prev_func->constant_pool->constants;
      cur_func = prev_func;

      stack[sp].u.i64_v = result_i64;

      break;
    }
    case RETURN_F32: {
      float_t result_f32;
      int32_t stack_offset;

      STACK_POP_F32(result_f32);
      stack_offset = fp + cur_func->args_size + cur_func->locals;
      prev_func = stack[stack_offset].u.function;
      sp = fp;
      pc = stack[stack_offset + 1].u.i32_v;
      fp = stack[stack_offset + 2].u.i32_v;
      code = prev_func->code;
      constant_pool = prev_func->constant_pool->constants;
      cur_func = prev_func;

      stack[sp].u.f32_v = result_f32;

      break;
    }
    case RETURN_F64: {
      double_t result_f64;
      int32_t stack_offset;

      STACK_POP_F64(result_f64);
      stack_offset = fp + cur_func->args_size + cur_func->locals;
      prev_func = stack[stack_offset].u.function;
      sp = fp;
      pc = stack[stack_offset + 1].u.i32_v;
      fp = stack[stack_offset + 2].u.i32_v;
      code = prev_func->code;
      constant_pool = prev_func->constant_pool->constants;
      cur_func = prev_func;

      stack[sp].u.f64_v = result_f64;

      break;
    }
    case RETURN_OBJECT: {
      GCObject *result_obj;
      int32_t stack_offset;

      STACK_POP_OBJECT(result_obj);
      stack_offset = fp + cur_func->args_size + cur_func->locals;
      prev_func = stack[stack_offset].u.function;
      sp = fp;
      pc = stack[stack_offset + 1].u.i32_v;
      fp = stack[stack_offset + 2].u.i32_v;
      code = prev_func->code;
      constant_pool = prev_func->constant_pool->constants;
      cur_func = prev_func;

      stack[sp].u.gc_obj = result_obj;
      stack[sp].is_gc_obj = true;

      break;
    }
    case INVOKE: {
      MethodInfo *next_func;

      next_func = stack[sp].u.function;
      // printf("invoke function: %s\n", next_func->name);
      // printf("function flag: %d\n", next_func->flag);
      if (next_func->flag == METHOD_FLAG_NATIVE_FUNCTION) {
        NativeMethod *native_function = &(next_func->native_method);
        if (native_function->function_pointer == NULL) {
          native_function->function_pointer =
              load_library_function(machine->state, native_function->lib_path,
                                    native_function->entry_point);
        }
        sp = sp - next_func->args_size;
        // printf("call native function from sp = %d\n", sp);
        // printf("try to invoke natvie function...\n");
        native_function->function_pointer(&(stack[sp])); // return value omitted
      } else {
        int32_t stack_offset;
        i32 cur_fp;

        // printf("func name = %s, sp = %d, args size = %d\n", next_func->name,
        // sp,
        //       next_func->args_size);
        cur_fp = fp;
        fp = sp - next_func->args_size;
        // printf("fp = %d\n", fp);
        stack_offset = fp + next_func->args_size + next_func->locals;
        stack[stack_offset].u.function = cur_func;
        stack[stack_offset + 1].u.i32_v = pc;
        stack[stack_offset + 2].u.i32_v = cur_fp;
        cur_func = next_func;
        pc = 0;
        sp = stack_offset + 2;
        code = next_func->code;
        constant_pool = next_func->constant_pool->constants;
      }
      break;
    }
      // case INVOKE: {
      //	next_func = stack[sp].u.function;
      //	if (next_func->is_native_function)
      //	{
      //		if (next_func->u.nv->is_loaded == false)
      //		{
      //			next_func->u.nv->function_pointer =
      // load_library_function( 				machine->state,
      // next_func->u.nv->lib_path,
      // next_func->u.nv->func_name);
      //			next_func->u.nv->is_loaded = true;
      //		}
      //		sp = sp - next_func->u.nv->args_size;
      //		//printf("call native function from sp = %d\n", sp);
      //		next_func->u.nv->function_pointer(&(stack[sp])); //
      // return value omitted
      //	}
      //	else
      //	{
      //		//printf("func name = %s, sp = %d, args size = %d\n",
      // next_func->name, sp, next_func->u.func_info->args_size);
      // cur_fp = fp; 		fp = sp - next_func->u.func->args_size;
      // stack_index = fp + next_func->u.func->args_size +
      // next_func->u.func->locals; 		stack[stack_index].u.function =
      // cur_func;
      //		stack[stack_index + 1].u.i32_v = pc;
      //		stack[stack_index + 2].u.i32_v = cur_fp;
      //		cur_func = next_func;
      //		pc = 0;
      //		sp = stack_index + 3;
      //		code = next_func->u.func->code;
      //		constant_pool = next_func->u.func->constant_pool;
      //	}
      //	break;
      //}
    case PUSH_FUNCTION: {
      u16 constant_pool_offset;
      MethodInfo *static_func;

      READ_U16(constant_pool_offset);
      static_func = get_static_func(
          machine, constant_pool[constant_pool_offset].method_ref);

      STACK_PUSH_FUNCTION(static_func);
      break;
    }
    case PUSH_METHOD: {
      GCObject *instance;
      MethodInfo *method;
      ClassInfo *current_type;
      u16 constant_pool_offset;
      u16 method_offset;

      instance = stack[sp].u.gc_obj;
      READ_U16(constant_pool_offset);
      READ_U16(method_offset);
      GET_METHOD(method, machine,
                 constant_pool[constant_pool_offset].method_ref);
      current_type = method->class_info;
      GET_VIRTUAL_METHOD(method, machine, instance->obj->type, current_type,
                         method_offset);
      STACK_PUSH_FUNCTION(method);
      break;
    }
    case NEW: {
      GCObject *new_instance;
      u16 constant_pool_offset;
      ClassInfo *class_info;
      int i;

      READ_U16(constant_pool_offset);
      class_info =
          get_class(machine, constant_pool[constant_pool_offset].class_ref);

      new_instance = (GCObject *)malloc(sizeof(GCObject));
      new_instance->obj = (Object *)malloc(sizeof(Object));
      new_instance->is_array = false;
      new_instance->obj->type = class_info;
      new_instance->obj->fields =
          malloc(sizeof(Value) * class_info->fields_count);
      new_instance->marked = 0;  // TO DO: garbage collection
      new_instance->next = NULL; // TO DO: garbage collection
      for (i = 0; i < class_info->fields_count; i++) {
        new_instance->obj->fields[i].is_gc_obj = false;
      }
      STACK_PUSH_OBJECT(new_instance);
      break;
    }
    case DUPLICATE: {
      sp++;
      stack[sp].u.gc_obj = stack[sp - 1].u.gc_obj;
      stack[sp].is_gc_obj = true;
      break;
    }
    case DUPLICATE_OFFSET: {
      u16 offset;

      READ_U16(offset);
      sp++;
      stack[sp].u.gc_obj = stack[sp - 1 - offset].u.gc_obj;
      stack[sp].is_gc_obj = true;
      break;
    }
    case NEW_ARRAY: {
      GCObject *new_array;
      int32_t length;

      switch (code[pc]) {
      case TYPE_I32: {
        pc = pc + 1;
        STACK_POP_I32(length);
        new_array = (GCObject *)malloc(sizeof(GCObject));
        new_array->is_array = true;
        new_array->arr = (Array *)malloc(sizeof(Array));
        new_array->arr->length = length;
        new_array->arr->u.i32_array =
            (int32_t *)malloc(sizeof(int32_t) * length);
        new_array->arr->type = ARRAY_TYPE_I32;
        STACK_PUSH_OBJECT(new_array);
        break;
      }
      case TYPE_I64: {
        pc = pc + 1;
        STACK_POP_I32(length);
        new_array = (GCObject *)malloc(sizeof(GCObject));
        new_array->is_array = true;
        new_array->arr = (Array *)malloc(sizeof(Array));
        new_array->arr->length = length;
        new_array->arr->u.i64_array =
            (int64_t *)malloc(sizeof(int64_t) * length);
        new_array->arr->type = ARRAY_TYPE_I64;
        STACK_PUSH_OBJECT(new_array);
        break;
      }
      case TYPE_F32: {
        pc = pc + 1;
        STACK_POP_I32(length);
        new_array = (GCObject *)malloc(sizeof(GCObject));
        new_array->is_array = true;
        new_array->arr = (Array *)malloc(sizeof(Array));
        new_array->arr->length = length;
        new_array->arr->u.f32_array =
            (float_t *)malloc(sizeof(float_t) * length);
        new_array->arr->type = ARRAY_TYPE_F32;
        STACK_PUSH_OBJECT(new_array);
        break;
      }
      case TYPE_F64: {
        pc = pc + 1;
        STACK_POP_I32(length);
        new_array = (GCObject *)malloc(sizeof(GCObject));
        new_array->is_array = true;
        new_array->arr = (Array *)malloc(sizeof(Array));
        new_array->arr->length = length;
        new_array->arr->u.f64_array =
            (double_t *)malloc(sizeof(double_t) * length);
        new_array->arr->type = ARRAY_TYPE_F64;
        STACK_PUSH_OBJECT(new_array);
        break;
      }
      case TYPE_OBJECT: {
        pc = pc + 1;
        STACK_POP_I32(length);
        new_array = (GCObject *)malloc(sizeof(GCObject));
        new_array->is_array = true;
        new_array->arr = (Array *)malloc(sizeof(Array));
        new_array->arr->length = length;
        new_array->arr->u.obj_array = malloc(sizeof(GCObject) * length);
        new_array->arr->type = ARRAY_TYPE_OBJECT;
        STACK_PUSH_OBJECT(new_array);
        break;
      }
      default: {
        fprintf(stderr, "wrong array type\n");
        vm_throw(machine->state, VM_ERROR_ARRAY_TYPE_CODE);
      }
      }
    }
    case ARRAY_LENGTH: {
      GCObject *array;

      STACK_POP_OBJECT(array);
      STACK_PUSH_I32(array->arr->length);
      break;
    }
      // case UP_CAST:{
      //   // TO DO: 移除 UP_CAST 指令。不需要该指令。
      // u16 constant_pool_offset;

      // READ_U16(constant_pool_offset);
      //   break;
      // }
      /*case UP_CAST: {
                                      if (code[pc] == 0)
                                      {
                                                                      pc++;
                                                                      READ_USHORT(index);
                                                                      i =
      find_virtual_tables(machine,
      &(machine->exe->interfaces[index]), stack[sp].u.gc_obj->class_index);
                                                                      stack[sp].u.gc_obj->v_table
      =
      &(machine->exe->interfaces[index].v_tables[i]);
                                      }
                                      else
                                      {
                                                                      pc++;
                                                                      READ_USHORT(index);
                                                                      stack[sp].u.gc_obj->v_table
      =
      &(machine->exe->classes[index].v_table);
                                      }
                                      break;
      }*/
    default: {
      fprintf(stderr, "unsupported operation code: %d\n", op);
      vm_throw(machine->state, VM_ERROR_OPCODE);
    }
    }
  }
}

void view_stack(Machine *machine, Value *stack, int sp, int fp) {
  printf("\n");
  for (int i = 0; i <= sp; i++) {
    if (i == fp) {
      printf("fp >>");
    }
    printf("%d: ", i);
    if (stack[i].is_gc_obj) {
      if (stack[i].u.gc_obj->is_array) {
        printf("[ARRAY]  [%s]",
               type_tag_to_string(stack[i].u.gc_obj->arr->type));
      } else {
        printf("[OBJECT]  %s", stack[i].u.gc_obj->obj->type->name);
      }
      printf("\n");
    } else {
      printf("[VALUE]  %d    %f\n", stack[i].u.i32_v, stack[i].u.f64_v);
    }
  }
}

void copy_string(int32_t *char_array, String *str_v) {
  int i;

  for (i = 0; i < str_v->length; i++) {
    char_array[i] = (int32_t)(str_v->characters[i]);
  }
}
void entry(Machine *machine) {
  i32 stack_index;
  MethodInfo *cur_func;
  Value *stack;
  // arguments ... (fp) | local variables ... | previous function | last pc |
  // last fp
  cur_func = machine->function;
  stack = machine->stack;
  stack_index = machine->fp + cur_func->args_size + cur_func->locals;
  stack[stack_index].u.function = NULL;
  stack[stack_index + 1].u.i32_v = machine->pc;
  stack[stack_index + 2].u.i32_v = machine->fp;
  machine->sp = stack_index + 2;
}