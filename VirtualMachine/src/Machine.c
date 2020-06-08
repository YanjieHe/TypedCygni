#include "Machine.h"
#include "Library.h"
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_WRITE(VALUE)                                                     \
  sp++;                                                                        \
  stack[sp].u.##VALUE = (VALUE);
#define STACK_READ(VALUE)                                                      \
  (VALUE) = stack[sp].u.##VALUE;                                               \
  sp--;
#define READ_USHORT(VALUE)                                                     \
  VALUE = (((uint16_t)code[pc]) << 8) + ((uint16_t)code[pc + 1]);              \
  pc = pc + 2;

Machine *create_machine(int stack_max_size, Executable *exe) {
  Machine *machine;
  int i;

  machine = (Machine *)malloc(sizeof(Machine));
  machine->stack_max_size = stack_max_size;
  machine->stack = (Value *)malloc(sizeof(Value) * stack_max_size);
  machine->exe = exe;
  machine->fp = -1;
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
  uint16_t offset;
  uint16_t index;
  int32_t i32_v;
  int64_t i64_v;
  float_t f32_v;
  double_t f64_v;
  Object *gc_obj;
  Function *prev_func;
  Function *cur_func;
  Function *next_func;
  // void* pointer;
  int stack_index;
  int32_t array_index;
  String *str_v;
  // Object* str_obj;
  int32_t cur_fp;
  int i;

  machine->function = machine->exe->entry;
  cur_func = machine->function;
  code = machine->function->u.func->code;
  constant_pool = machine->function->u.func->constant_pool;
  stack = machine->stack;
  pc = 0;
  fp = 0;

  // arguments ... (fp) | local variables ... | previous function | last pc |
  // last fp
  stack_index = fp + cur_func->u.func->args_size + cur_func->u.func->locals;
  stack[stack_index + 1].u.i32_v = pc;
  stack[stack_index + 2].u.i32_v = fp;
  sp = stack_index + 3;

  while (pc < cur_func->u.func->code_len) {
    op = code[pc];
    view_stack(machine, stack, sp);
    printf("function: %s, op = %s, sp = %d, fp = %d, pc = %d\n", cur_func->name,
           opcode_info[op][0], sp, fp, pc);
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
      READ_USHORT(index);
      i32_v = constant_pool[index].u.i32_v;
      STACK_WRITE(i32_v);
      break;
    }
    case PUSH_I64: {
      READ_USHORT(index);
      i64_v = constant_pool[index].u.i64_v;
      STACK_WRITE(i64_v);
      break;
    }
    case PUSH_F32: {
      READ_USHORT(index);
      f32_v = constant_pool[index].u.f32_v;
      STACK_WRITE(f32_v);
      break;
    }
    case PUSH_F64: {
      READ_USHORT(index);
      f64_v = constant_pool[index].u.f64_v;
      STACK_WRITE(f64_v);
      break;
    }
    case PUSH_STRING: {
      READ_USHORT(index);
      str_v = constant_pool[index].u.str_v;

      // gc_obj = malloc(sizeof(Object));
      // gc_obj->is_array = false;
      // gc_obj->next = NULL;
      // gc_obj->class_index = 65535; // the class index of string
      // gc_obj->u.fields = malloc(sizeof(Value) * 1);
      // gc_obj->u.fields[0].is_gc_obj = true;

      // str_obj = malloc(sizeof(Object));
      // str_obj->is_array = true;
      // str_obj->next = NULL;
      // str_obj->u.array = malloc(sizeof(Array));
      // str_obj->u.array->length = str_v->length;
      // str_obj->u.array->u.i32_array = malloc(sizeof(int32_t) *
      // str_v->length);

      gc_obj = malloc(sizeof(Object));
      gc_obj->is_array = true;
      gc_obj->next = NULL;
      gc_obj->class_index = 0;
      gc_obj->u.array = malloc(sizeof(Array));
      gc_obj->u.array->length = str_v->length;
      gc_obj->u.array->type = TYPE_I32;
      gc_obj->u.array->u.i32_array = malloc(sizeof(int32_t) * str_v->length);
      copy_string(gc_obj->u.array->u.i32_array, str_v);

      // gc_obj->u.fields[0].u.gc_obj = str_obj;
      STACK_WRITE(gc_obj);
      stack[sp].is_gc_obj = true;
      break;
    }
    case PUSH_LOCAL_I32: {
      READ_USHORT(offset);
      i32_v = stack[fp + offset].u.i32_v;
      STACK_WRITE(i32_v);
      break;
    }
    case PUSH_LOCAL_I64: {
      READ_USHORT(offset);
      i64_v = stack[fp + offset].u.i64_v;
      STACK_WRITE(i64_v);
      break;
    }
    case PUSH_LOCAL_F32: {
      READ_USHORT(offset);
      f32_v = stack[fp + offset].u.f32_v;
      STACK_WRITE(f32_v);
      break;
    }
    case PUSH_LOCAL_F64: {
      READ_USHORT(offset);
      f64_v = stack[fp + offset].u.f64_v;
      STACK_WRITE(f64_v);
      break;
    }
    case PUSH_LOCAL_OBJECT: {
      READ_USHORT(offset);
      // printf("fp = %d, offset = %d\n", fp, offset);
      gc_obj = stack[fp + offset].u.gc_obj;
      STACK_WRITE(gc_obj);
      stack[sp].is_gc_obj = true;
      /*printf("object class name = %s\n",
      machine->exe->classes[(stack[sp]).u.gc_obj->class_index].name);
      printf("field count = %d\n",
      machine->exe->classes[(stack[sp]).u.gc_obj->class_index].n_fields);
      printf("is local object an object? %d\n",
              (stack[sp]).u.gc_obj->u.fields[0].is_gc_obj);*/
      // if ((stack[sp]).u.gc_obj->u.fields[0].is_gc_obj)
      //{
      //	printf("is array? %d\n",
      //(stack[sp]).u.gc_obj->u.fields[0].u.i32_v); 	printf("is array? %d\n",
      //(stack[sp]).u.gc_obj->u.fields[0].u.gc_obj->class_index);
      //}
      break;
    }
    case POP_LOCAL_I32: {
      READ_USHORT(offset);
      STACK_READ(i32_v);
      stack[fp + offset].u.i32_v = i32_v;
      break;
    }
    case POP_LOCAL_I64: {
      READ_USHORT(offset);
      STACK_READ(i64_v);
      stack[fp + offset].u.i64_v = i64_v;
      break;
    }
    case POP_LOCAL_F32: {
      READ_USHORT(offset);
      STACK_READ(f32_v);
      stack[fp + offset].u.f32_v = f32_v;
      break;
    }
    case POP_LOCAL_F64: {
      READ_USHORT(offset);
      STACK_READ(i64_v);
      stack[fp + offset].u.i64_v = i64_v;
      break;
    }
    case POP_LOCAL_OBJECT: {
      READ_USHORT(offset);
      STACK_READ(gc_obj);
      stack[sp + 1].is_gc_obj = false;
      stack[fp + offset].u.gc_obj = gc_obj;
      stack[fp + offset].is_gc_obj = true;
      break;
    }
    case PUSH_STATIC_I32: {
      READ_USHORT(index);
      READ_USHORT(offset);
      i32_v = machine->exe->modules[index].variables[offset].u.i32_v;
      STACK_WRITE(i32_v);
      break;
    }
    case PUSH_STATIC_I64: {
      READ_USHORT(index);
      READ_USHORT(offset);
      i64_v = machine->exe->modules[index].variables[offset].u.i64_v;
      STACK_WRITE(i64_v);
      break;
    }
    case PUSH_STATIC_F32: {
      READ_USHORT(index);
      READ_USHORT(offset);
      f32_v = machine->exe->modules[index].variables[offset].u.f32_v;
      STACK_WRITE(f32_v);
      break;
    }
    case PUSH_STATIC_F64: {
      READ_USHORT(index);
      READ_USHORT(offset);
      f64_v = machine->exe->modules[index].variables[offset].u.f64_v;
      STACK_WRITE(f64_v);
      break;
    }
    case PUSH_STATIC_OBJECT: {
      READ_USHORT(index);
      READ_USHORT(offset);
      gc_obj = machine->exe->modules[index].variables[offset].u.gc_obj;
      STACK_WRITE(gc_obj);
      stack[sp].is_gc_obj = true;
      break;
    }
    case POP_STATIC_I32: {
      READ_USHORT(index);
      READ_USHORT(offset);
      STACK_READ(i32_v);
      machine->exe->modules[index].variables[offset].u.i32_v = i32_v;
      break;
    }
    case POP_STATIC_I64: {
      READ_USHORT(index);
      READ_USHORT(offset);
      STACK_READ(i64_v);
      machine->exe->modules[index].variables[offset].u.i64_v = i64_v;
      break;
    }
    case POP_STATIC_F32: {
      READ_USHORT(index);
      READ_USHORT(offset);
      STACK_READ(f32_v);
      machine->exe->modules[index].variables[offset].u.f32_v = f32_v;
      break;
    }
    case POP_STATIC_F64: {
      READ_USHORT(index);
      READ_USHORT(offset);
      STACK_READ(f64_v);
      machine->exe->modules[index].variables[offset].u.f64_v = f64_v;
      break;
    }
    case POP_STATIC_OBJECT: {
      READ_USHORT(index);
      READ_USHORT(offset);
      STACK_READ(gc_obj);
      stack[sp + 1].is_gc_obj = false;
      machine->exe->modules[index].variables[offset].u.gc_obj = gc_obj;
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
      gc_obj = stack[sp].u.gc_obj;
      READ_USHORT(offset);
      // printf("PUSH_FIELD_I32\n");
      // printf("is gc obj: %d\n", stack[sp].is_gc_obj);
      // printf("offset = %d\n", offset);
      // view_stack(stack, sp);
      // printf("class index = %d\n", gc_obj->class_index);
      // printf("field name: %s\n",
      // machine->exe->classes[gc_obj->class_index].field_names[offset]);
      // printf("field int: %d\n", gc_obj->u.fields[offset].u.i32_v);
      stack[sp].u.i32_v = gc_obj->u.fields[offset].u.i32_v;
      stack[sp].is_gc_obj = false;
      break;
    }
    case PUSH_FIELD_I64: {
      gc_obj = stack[sp].u.gc_obj;
      READ_USHORT(offset);
      stack[sp].u.i64_v = gc_obj->u.fields[offset].u.i64_v;
      stack[sp].is_gc_obj = false;
      break;
    }
    case PUSH_FIELD_F32: {
      gc_obj = stack[sp].u.gc_obj;
      READ_USHORT(offset);
      stack[sp].u.f32_v = gc_obj->u.fields[offset].u.f32_v;
      stack[sp].is_gc_obj = false;
      break;
    }
    case PUSH_FIELD_F64: {
      gc_obj = stack[sp].u.gc_obj;
      READ_USHORT(offset);
      stack[sp].u.f64_v = gc_obj->u.fields[offset].u.f64_v;
      stack[sp].is_gc_obj = false;
      break;
    }
    case PUSH_FIELD_OBJECT: {
      gc_obj = stack[sp].u.gc_obj;
      READ_USHORT(offset);
      // printf("rect address %ld\n", (long)gc_obj);
      // printf("address %ld\n", (long)gc_obj->u.fields[offset].u.gc_obj);
      stack[sp].u.gc_obj = gc_obj->u.fields[offset].u.gc_obj;
      stack[sp].is_gc_obj = true;
      // printf("address %ld\n", (long)stack[sp].u.gc_obj);
      break;
    }
    case POP_FIELD_I32: {
      // printf("is gc obj? %d\n", stack[sp - 1].is_gc_obj);
      gc_obj = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_USHORT(offset);
      // printf("offset = %d\n", offset);
      gc_obj->u.fields[offset].u.i32_v = stack[sp].u.i32_v;
      // printf("POP_FIELD_I32 gc_obj->u.fields[offset].u.i32 = %d\n",
      // gc_obj->u.fields[offset].u.i32_v);
      sp = sp - 2;
      break;
    }
    case POP_FIELD_I64: {
      gc_obj = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_USHORT(offset);
      gc_obj->u.fields[offset].u.i64_v = stack[sp].u.i64_v;
      sp = sp - 2;
      break;
    }
    case POP_FIELD_F32: {
      gc_obj = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_USHORT(offset);
      gc_obj->u.fields[offset].u.f32_v = stack[sp].u.f32_v;
      sp = sp - 2;
      break;
    }
    case POP_FIELD_F64: {
      gc_obj = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_USHORT(offset);
      gc_obj->u.fields[offset].u.f64_v = stack[sp].u.f64_v;
      sp = sp - 2;
      break;
    }
    case POP_FIELD_OBJECT: {
      gc_obj = stack[sp - 1].u.gc_obj;
      stack[sp - 1].is_gc_obj = false;
      READ_USHORT(offset);
      // printf("offset = %d\n", offset);
      // printf("class index = %d\n", gc_obj->class_index);
      // printf("field name: %s\n",
      // machine->exe->classes[gc_obj->class_index].field_names[offset]);
      // printf("field string: is array? %d\n",
      // stack[sp].u.gc_obj->u.fields[0].u.gc_obj->is_array); printf("rect
      // address %ld\n", (long)stack[sp - 1].u.gc_obj); printf("rect address
      // %ld\n", (long)gc_obj); printf("address %ld\n",
      // (long)stack[sp].u.gc_obj);
      gc_obj->u.fields[offset].u.gc_obj = stack[sp].u.gc_obj;
      stack[sp].is_gc_obj = false;
      gc_obj->u.fields[offset].is_gc_obj = true;
      // printf("address %ld\n", (long)gc_obj->u.fields[offset].u.gc_obj);
      // printf("field 'name' class %s\n",
      //	machine->exe->classes[gc_obj->u.fields[offset].u.gc_obj->class_index].name);
      sp = sp - 2;
      break;
    }
    case PUSH_ARRAY_I32: {
      STACK_READ(gc_obj);
      STACK_READ(i32_v);
      gc_obj = stack[sp].u.gc_obj;
      i32_v = gc_obj->u.array->u.i32_array[i32_v];
      STACK_WRITE(i32_v);
      break;
    }
    case PUSH_ARRAY_I64: {
      STACK_READ(gc_obj);
      STACK_READ(i32_v);
      gc_obj = stack[sp].u.gc_obj;
      i64_v = gc_obj->u.array->u.i64_array[i32_v];
      STACK_WRITE(i64_v);
      break;
    }
    case PUSH_ARRAY_F32: {
      STACK_READ(gc_obj);
      STACK_READ(i32_v);
      gc_obj = stack[sp].u.gc_obj;
      f32_v = gc_obj->u.array->u.f32_array[i32_v];
      STACK_WRITE(f32_v);
      break;
    }
    case PUSH_ARRAY_F64: {
      STACK_READ(gc_obj);
      STACK_READ(i32_v);
      gc_obj = stack[sp].u.gc_obj;
      f64_v = gc_obj->u.array->u.f64_array[i32_v];
      STACK_WRITE(f64_v);
      break;
    }
    case PUSH_ARRAY_OBJECT: {
      STACK_READ(gc_obj);
      STACK_READ(i32_v);
      gc_obj = stack[sp].u.gc_obj;
      gc_obj = gc_obj->u.array->u.obj_array[i32_v];
      STACK_WRITE(gc_obj);
      stack[sp].is_gc_obj = true;
      break;
    }
    case POP_ARRAY_I32: {
      STACK_READ(i32_v);
      array_index = i32_v;
      STACK_READ(gc_obj); // array
      STACK_READ(i32_v);  // value
      gc_obj->u.array->u.i32_array[array_index] = i32_v;
      break;
    }
    case POP_ARRAY_I64: {
      STACK_READ(i32_v);
      array_index = i32_v;
      STACK_READ(gc_obj); // array
      STACK_READ(i64_v);  // value
      gc_obj->u.array->u.i64_array[array_index] = i64_v;
      break;
    }
    case POP_ARRAY_F32: {
      STACK_READ(i32_v);
      array_index = i32_v;
      STACK_READ(gc_obj);
      STACK_READ(f32_v);
      gc_obj->u.array->u.f32_array[array_index] = f32_v;
      break;
    }
    case POP_ARRAY_F64: {
      STACK_READ(i32_v);
      array_index = i32_v;
      STACK_READ(gc_obj);
      STACK_READ(f64_v);
      gc_obj->u.array->u.f64_array[array_index] = f64_v;
      break;
    }
    case POP_ARRAY_OBJECT: {
      STACK_READ(i32_v);
      array_index = i32_v;
      STACK_READ(gc_obj);
      gc_obj->u.array->u.obj_array[array_index] = stack[sp].u.gc_obj;
      stack[sp].is_gc_obj = false;
      sp--;
      break;
    }
    case JUMP: {
      pc = bytes_to_u16(code, pc);
      break;
    }
    case JUMP_IF_TRUE: {
      STACK_READ(i32_v);
      if (i32_v) {
        pc = bytes_to_u16(code, pc);
      } else {
        pc = pc + 2;
      }
      break;
    }
    case JUMP_IF_FALSE: {
      STACK_READ(i32_v);
      if (i32_v) {
        pc = pc + 2;
      } else {
        pc = bytes_to_u16(code, pc);
      }
      break;
    }
    case RETURN_I32: {
      STACK_READ(i32_v);
      stack_index = fp + cur_func->u.func->args_size + cur_func->u.func->locals;
      prev_func = stack[stack_index].u.function;
      sp = fp;
      pc = stack[stack_index + 1].u.i32_v;
      fp = stack[stack_index + 2].u.i32_v;
      code = prev_func->u.func->code;
      constant_pool = prev_func->u.func->constant_pool;
      cur_func = prev_func;

      stack[sp].u.i32_v = i32_v;

      break;
    }
    case RETURN_I64: {
      STACK_READ(i64_v);
      stack_index = fp + cur_func->u.func->args_size + cur_func->u.func->locals;
      prev_func = stack[stack_index].u.function;
      sp = fp;
      pc = stack[stack_index + 1].u.i32_v;
      fp = stack[stack_index + 2].u.i32_v;
      code = prev_func->u.func->code;
      constant_pool = prev_func->u.func->constant_pool;
      cur_func = prev_func;

      stack[sp].u.i64_v = i64_v;
      break;
    }
    case RETURN_F32: {
      STACK_READ(f32_v);
      stack_index = fp + cur_func->u.func->args_size + cur_func->u.func->locals;
      prev_func = stack[stack_index].u.function;
      sp = fp;
      pc = stack[stack_index + 1].u.i32_v;
      fp = stack[stack_index + 2].u.i32_v;
      code = prev_func->u.func->code;
      constant_pool = prev_func->u.func->constant_pool;
      cur_func = prev_func;

      stack[sp].u.f32_v = f32_v;
      break;
    }
    case RETURN_F64: {
      STACK_READ(f64_v);
      stack_index = fp + cur_func->u.func->args_size + cur_func->u.func->locals;
      prev_func = stack[stack_index].u.function;
      sp = fp;
      pc = stack[stack_index + 1].u.i32_v;
      fp = stack[stack_index + 2].u.i32_v;
      code = prev_func->u.func->code;
      constant_pool = prev_func->u.func->constant_pool;
      cur_func = prev_func;

      stack[sp].u.f64_v = f64_v;
      break;
    }
    case RETURN_OBJECT: {
      STACK_READ(gc_obj);
      stack_index = fp + cur_func->u.func->args_size + cur_func->u.func->locals;
      prev_func = stack[stack_index].u.function;
      sp = fp;
      pc = stack[stack_index + 1].u.i32_v;
      fp = stack[stack_index + 2].u.i32_v;
      code = prev_func->u.func->code;
      constant_pool = prev_func->u.func->constant_pool;
      cur_func = prev_func;

      stack[sp].u.gc_obj = gc_obj;
      break;
    }
    case INVOKE_FUNCTION: {
      READ_USHORT(index);
      READ_USHORT(offset);
      next_func = machine->exe->modules[index].functions[offset];
      sp++;
      stack[sp].u.function = next_func;
      if (next_func->is_native_function) {
        if (next_func->u.nv->is_loaded == false) {
          next_func->u.nv->function_pointer =
              load_library_function(machine->state, next_func->u.nv->lib_path,
                                    next_func->u.nv->func_name);
          next_func->u.nv->is_loaded = true;
        }
        sp = sp - next_func->u.nv->args_size;
        // printf("call native function from sp = %d\n", sp);
        next_func->u.nv->function_pointer(&(stack[sp])); // return value omitted
      } else {
        printf("func name = %s, sp = %d, args size = %d\n", next_func->name, sp,
               next_func->u.func->args_size);
        cur_fp = fp;
        fp = sp - next_func->u.func->args_size;
        printf("fp = %d\n", fp);
        stack_index =
            fp + next_func->u.func->args_size + next_func->u.func->locals;
        stack[stack_index].u.function = cur_func;
        stack[stack_index + 1].u.i32_v = pc;
        stack[stack_index + 2].u.i32_v = cur_fp;
        cur_func = next_func;
        pc = 0;
        sp = stack_index + 3;
        code = next_func->u.func->code;
        constant_pool = next_func->u.func->constant_pool;
      }
      break;
    }
    case INVOKE_METHOD: {
      READ_USHORT(index);
      READ_USHORT(offset);

      next_func = machine->exe->classes[index].v_table.methods[offset];
      sp++;
      stack[sp].u.function = next_func;

      cur_fp = fp;
      fp = sp - next_func->u.func->args_size;
      stack_index =
          fp + next_func->u.func->args_size + next_func->u.func->locals;
      stack[stack_index].u.function = cur_func;
      stack[stack_index + 1].u.i32_v = pc;
      stack[stack_index + 2].u.i32_v = cur_fp;
      cur_func = next_func;
      pc = 0;
      sp = stack_index + 3;
      code = next_func->u.func->code;
      constant_pool = next_func->u.func->constant_pool;
      break;
    }
      // case INVOKE: {
      //	next_func = stack[sp].u.function;
      //	if (next_func->is_native_function)
      //	{
      //		if (next_func->u.nv->is_loaded == false)
      //		{
      //			next_func->u.nv->function_pointer =
      //load_library_function( 				machine->state, 				next_func->u.nv->lib_path,
      //				next_func->u.nv->func_name);
      //			next_func->u.nv->is_loaded = true;
      //		}
      //		sp = sp - next_func->u.nv->args_size;
      //		//printf("call native function from sp = %d\n", sp);
      //		next_func->u.nv->function_pointer(&(stack[sp])); //
      //return value omitted
      //	}
      //	else
      //	{
      //		//printf("func name = %s, sp = %d, args size = %d\n",
      //next_func->name, sp, next_func->u.func_info->args_size); 		cur_fp = fp; 		fp
      //= sp - next_func->u.func->args_size; 		stack_index = fp +
      //next_func->u.func->args_size + next_func->u.func->locals;
      //		stack[stack_index].u.function = cur_func;
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
      /*case PUSH_FUNCTION: {
              READ_USHORT(index);
              READ_USHORT(offset);
              sp++;
              stack[sp].u.function =
      machine->exe->modules[index].functions[offset]; break;
      }
      case PUSH_METHOD: {
              READ_USHORT(index);
              READ_USHORT(offset);
              sp++;
              stack[sp].u.function =
      machine->exe->classes[index].methods[offset]; break;
      }*/
    case NEW: {
      READ_USHORT(index);
      gc_obj = (Object *)malloc(sizeof(Object));
      gc_obj->is_array = false;
      gc_obj->class_index = index;
      gc_obj->u.fields =
          malloc(sizeof(Value) * machine->exe->classes[index].n_fields);
      gc_obj->next = NULL; // TO DO: garbage collection
      for (i = 0; i < machine->exe->classes[index].n_fields; i++) {
        gc_obj->u.fields[0].is_gc_obj = false;
      }
      STACK_WRITE(gc_obj);
      stack[sp].is_gc_obj = true;
      break;
    }
    case DUPLICATE: {
      sp++;
      stack[sp].u.gc_obj = stack[sp - 1].u.gc_obj;
      stack[sp].is_gc_obj = true;
      break;
    }
    case DUPLICATE_OFFSET: {
      READ_USHORT(offset);
      sp++;
      stack[sp].u.gc_obj = stack[sp - 1 - offset].u.gc_obj;
      stack[sp].is_gc_obj = true;
      break;
    }
    case NEW_ARRAY: {
      switch (code[pc]) {
      case TYPE_I32: {
        pc = pc + 1;
        STACK_READ(i32_v);
        gc_obj = (Object *)malloc(sizeof(Object));
        gc_obj->is_array = true;
        gc_obj->class_index = 0;
        gc_obj->next = NULL;
        gc_obj->u.array = (Array *)malloc(sizeof(Array));
        gc_obj->u.array->length = i32_v;
        gc_obj->u.array->u.i32_array = malloc(sizeof(int32_t) * i32_v);
        gc_obj->u.array->type = ARRAY_TYPE_I32;
        STACK_WRITE(gc_obj);
        break;
      }
      case TYPE_I64: {
        pc = pc + 1;
        STACK_READ(i32_v);
        gc_obj = (Object *)malloc(sizeof(Object));
        gc_obj->is_array = true;
        gc_obj->class_index = 0;
        gc_obj->next = NULL;
        gc_obj->u.array = (Array *)malloc(sizeof(Array));
        gc_obj->u.array->length = i32_v;
        gc_obj->u.array->u.i64_array = malloc(sizeof(int64_t) * i32_v);
        gc_obj->u.array->type = ARRAY_TYPE_I64;
        STACK_WRITE(gc_obj);
        break;
      }
      case TYPE_F32: {
        pc = pc + 1;
        STACK_READ(i32_v);
        gc_obj = (Object *)malloc(sizeof(Object));
        gc_obj->is_array = true;
        gc_obj->class_index = 0;
        gc_obj->next = NULL;
        gc_obj->u.array = (Array *)malloc(sizeof(Array));
        gc_obj->u.array->length = i32_v;
        gc_obj->u.array->u.f32_array = malloc(sizeof(float_t) * i32_v);
        gc_obj->u.array->type = ARRAY_TYPE_F32;
        STACK_WRITE(gc_obj);
        break;
      }
      case TYPE_F64: {
        pc = pc + 1;
        STACK_READ(i32_v);
        gc_obj = (Object *)malloc(sizeof(Object));
        gc_obj->is_array = true;
        gc_obj->class_index = 0;
        gc_obj->next = NULL;
        gc_obj->u.array = (Array *)malloc(sizeof(Array));
        gc_obj->u.array->length = i32_v;
        gc_obj->u.array->u.i64_array = malloc(sizeof(double_t) * i32_v);
        gc_obj->u.array->type = ARRAY_TYPE_F64;
        STACK_WRITE(gc_obj);
        break;
      }
      case TYPE_OBJECT: {
        pc = pc + 1;
        STACK_READ(i32_v);
        gc_obj = (Object *)malloc(sizeof(Object));
        gc_obj->is_array = true;
        gc_obj->class_index = 0;
        gc_obj->next = NULL;
        gc_obj->u.array = (Array *)malloc(sizeof(Array));
        gc_obj->u.array->length = i32_v;
        gc_obj->u.array->u.obj_array = malloc(sizeof(Object) * i32_v);
        gc_obj->u.array->type = ARRAY_TYPE_OBJECT;
        STACK_WRITE(gc_obj);
        break;
      }
      default: {
        fprintf(stderr, "wrong array type\n");
        vm_throw(machine->state, VM_ERROR_ARRAY_TYPE_CODE);
      }
      }
    }
    case ARRAY_LENGTH: {
      STACK_READ(gc_obj);
      sp++;
      stack[sp].u.i32_v = gc_obj->u.array->length;
      stack[sp].is_gc_obj = false;
      break;
    }
    /*case UP_CAST: {
            if (code[pc] == 0)
            {
                    pc++;
                    READ_USHORT(index);
                    i = find_virtual_tables(machine,
    &(machine->exe->interfaces[index]), stack[sp].u.gc_obj->class_index);
                    stack[sp].u.gc_obj->v_table =
    &(machine->exe->interfaces[index].v_tables[i]);
            }
            else
            {
                    pc++;
                    READ_USHORT(index);
                    stack[sp].u.gc_obj->v_table =
    &(machine->exe->classes[index].v_table);
            }
            break;
    }*/
    default: {
      fprintf(stderr, "unsupported operation code: ");
      fprintf(stderr, "%d\n", op);
      vm_throw(machine->state, VM_ERROR_OPCODE);
    }
    }
  }
}

void view_stack(Machine *machine, Value *stack, int sp) {
  for (int i = 0; i <= sp; i++) {
    printf("%d: ", i);
    if (stack[i].is_gc_obj) {
      if (stack[i].u.gc_obj->is_array) {
        printf("[ARRAY]  [%s]",
               type_tag_to_string(stack[i].u.gc_obj->u.array->type));
      } else {
        printf("[OBJECT]  %s",
               machine->exe->classes[stack[i].u.gc_obj->class_index].name);
      }
      printf("\n");
    } else {
      printf("[VALUE]  %d\n", stack[i].u.i32_v);
    }
  }
  printf("\n");
}

void copy_string(int32_t *char_array, String *str_v) {
  int i;

  for (i = 0; i < str_v->length; i++) {
    char_array[i] = (int32_t)(str_v->characters[i]);
  }
}

int find_virtual_table(ClassInfo *classInfo, int interface_index) {
  int i;
  int low;
  int high;
  int mid;

  i = 0;
  low = 0;
  high = classInfo->n_interface - 1;

  while (low <= high) {
    mid = (low + high) / 2;
    if (classInfo->interface_index_list[mid] < interface_index) {
      low = mid + 1;
    } else if (classInfo->interface_index_list[mid] > interface_index) {
      high = mid - 1;
    } else {
      return mid;
    }
  }
  return (-1);
}
