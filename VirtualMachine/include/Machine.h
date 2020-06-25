#ifndef VM_MACHINE_H
#define VM_MACHINE_H
#include "cygni.h"
#include "executable.h"
#include "state.h"
#include "opcode.h"

typedef struct {
  int stack_max_size;
  Value *stack;
  MethodInfo *function;
  Executable *exe;
  int fp;
  int sp;
  int pc;
  State *state;
} Machine;

Machine *create_machine(int stack_max_size, Executable *exe);

void run(Machine *machine);

void view_stack(Machine *machine, Value *stack, int sp);

void copy_string(int32_t *char_array, String *str_v);

static StaticVarInfo *get_static_var(Machine *machine,
                                     StaticVarRef *static_var_ref);

static MethodInfo *get_static_func(Machine *machine,
                                   MethodRef *static_func_ref);

static MethodInfo *get_virtual_method(Machine *machine, ClassInfo *original_type,
                              ClassInfo *current_type, u16 method_offset);

static ClassInfo *get_class(Machine *machine, ClassRef *class_ref);

static StaticVarInfo *get_static_var(Machine *machine,
                                     StaticVarRef *static_var_ref) {
  if (static_var_ref->static_var) {
    return static_var_ref->static_var;
  } else {
    StaticVarMap *static_var_map_pair;

    HASH_FIND_STR(machine->exe->static_var_map, static_var_ref->name,
                  static_var_map_pair);

    if (static_var_map_pair) {
      static_var_ref->static_var = static_var_map_pair->static_var;
      return static_var_map_pair->static_var;
    } else {
      vm_throw(machine->state, VM_ERROR_STATIC_VAR_NOT_FOUND);
      return NULL;
    }
  }
}

static MethodInfo *get_static_func(Machine *machine,
                                   MethodRef *static_func_ref) {
  if (static_func_ref->method_info) {
    return static_func_ref->method_info;
  } else {
    StaticFunctionMap *static_function_map_pair;

    HASH_FIND_STR(machine->exe->static_function_map, static_func_ref->name,
                  static_function_map_pair);

    if (static_function_map_pair) {
      static_func_ref->method_info = static_function_map_pair->function;
      return static_function_map_pair->function;
    } else {
      vm_throw(machine->state, VM_ERROR_STATIC_FUNCTION_NOT_FOUND);
      return NULL;
    }
  }
}

static ClassInfo *get_class(Machine *machine, ClassRef *class_ref) {
  if (class_ref->class_info) {
    return class_ref->class_info;
  } else {
    ClassMap *class_map_pair;

    HASH_FIND_STR(machine->exe->class_map, class_ref->name, class_map_pair);

    if (class_map_pair) {
      class_ref->class_info = class_map_pair->class_info;
      return class_map_pair->class_info;
    } else {
      vm_throw(machine->state, VM_ERROR_CLASS_NOT_FOUND);
      return NULL;
    }
  }
}

static MethodInfo *get_method(Machine *machine, MethodRef *method_ref) {
  if (method_ref->method_info) {
    return method_ref->method_info;
  } else {
    MethodMap *method_map_pair;

    HASH_FIND_STR(machine->exe->class_map, method_ref->name, method_map_pair);

    if (method_map_pair) {
      method_ref->method_info = method_map_pair->method_info;
      return method_map_pair->method_info;
    } else {
      vm_throw(machine->state, VM_ERROR_FAIL_TO_FIND_METHOD);
      return NULL;
    }
  }
}

static MethodInfo *get_virtual_method(Machine *machine, ClassInfo *original_type,
                              ClassInfo *current_type, u16 method_offset) {
  int i;
  for (i = 0; i < original_type->virtual_tables_count; i++) {
    ClassInfo *class_info;
    VirtualTable *virtual_table = &(original_type->virtual_tables[i]);

    class_info = get_class(machine, &(virtual_table->class_ref));
    if (class_info == current_type) {
      return get_method(machine, &(virtual_table->methods[method_offset]));
    }
  }
  vm_throw(machine->state, VM_ERROR_FAIL_TO_FIND_CLASS_IN_VIRTUAL_TABLE);
  return NULL;
}

// int find_virtual_table(ClassInfo *classInfo, int interface_index);
#endif // VM_MACHINE_H