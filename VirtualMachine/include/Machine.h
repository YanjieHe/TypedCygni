#ifndef VM_MACHINE_H
#define VM_MACHINE_H
#include "cygni.h"
#include "executable.h"
#include "opcode.h"
#include "state.h"

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

void view_stack(Machine *machine, Value *stack, int sp, int fp);

void copy_string(int32_t *char_array, String *str_v);

void entry(Machine *machine);

static StaticVarInfo *get_static_var(Machine *machine,
                                     StaticVarRef *static_var_ref);

static MethodInfo *get_static_func(Machine *machine,
                                   MethodRef *static_func_ref);

static MethodInfo *get_virtual_method(Machine *machine,
                                      ClassInfo *original_type,
                                      ClassInfo *current_type,
                                      u16 method_offset);

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
      printf("cannot find static function '%s'\n", static_func_ref->name);
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

    printf("TRY TO FIND METHOD: %s\n", method_ref->name);
    HASH_FIND_STR(machine->exe->method_map, method_ref->name, method_map_pair);

    if (method_map_pair) {
      method_ref->method_info = method_map_pair->method_info;
      printf("METHOD FOUND\n");
      return method_map_pair->method_info;
    } else {
      printf("METHOD NOT FOUND\n");
      vm_throw(machine->state, VM_ERROR_FAIL_TO_FIND_METHOD);
      return NULL;
    }
  }
}

static MethodInfo *get_virtual_method(Machine *machine,
                                      ClassInfo *original_type,
                                      ClassInfo *current_type,
                                      u16 method_offset) {
  int i;
  if (original_type == current_type) {
    return &(original_type->methods[method_offset]);
  } else {
    for (i = 0; i < original_type->virtual_tables_count; i++) {
      ClassInfo *class_info;
      VirtualTable *virtual_table = &(original_type->virtual_tables[i]);

      class_info = get_class(machine, &(virtual_table->class_ref));
      printf("class info = %s\n", class_info->name);
      if (class_info == current_type) {
        return get_method(machine, &(virtual_table->methods[method_offset]));
      }
    }
    printf("fail to find %s\n", current_type->name);
    vm_throw(machine->state, VM_ERROR_FAIL_TO_FIND_CLASS_IN_VIRTUAL_TABLE);
    return NULL;
  }
}

#define GET_METHOD(METHOD_INFO, MACHINE, METHOD_REF)                           \
  {                                                                            \
    if ((METHOD_REF)->method_info) {                                           \
      (METHOD_INFO) = (METHOD_REF)->method_info;                               \
    } else {                                                                   \
      MethodMap *method_map_pair;                                              \
      HASH_FIND_STR((MACHINE)->exe->method_map, (METHOD_REF)->name,            \
                    method_map_pair);                                          \
      if (method_map_pair) {                                                   \
        (METHOD_REF)->method_info = method_map_pair->method_info;              \
        (METHOD_INFO) = method_map_pair->method_info;                          \
      } else {                                                                 \
        vm_throw((MACHINE)->state, VM_ERROR_FAIL_TO_FIND_METHOD);              \
        (METHOD_INFO) = NULL;                                                  \
      }                                                                        \
    }                                                                          \
  }

#define GET_VIRTUAL_METHOD(METHOD_INFO, MACHINE, ORIGINAL_TYPE, CURRENT_TYPE,  \
                           METHOD_OFFSET)                                      \
  {                                                                            \
    int i;                                                                     \
    if ((ORIGINAL_TYPE) == (CURRENT_TYPE)) {                                   \
      METHOD_INFO = &((ORIGINAL_TYPE)->methods[(METHOD_OFFSET)]);              \
    } else {                                                                   \
      for (i = 0; i < (ORIGINAL_TYPE)->virtual_tables_count; i++) {            \
        ClassInfo *class_info;                                                 \
        VirtualTable *virtual_table = &((ORIGINAL_TYPE)->virtual_tables[i]);   \
        class_info = get_class((MACHINE), &(virtual_table->class_ref));        \
        if (class_info == (CURRENT_TYPE)) {                                    \
          GET_METHOD(METHOD_INFO, machine,                                     \
                     &(virtual_table->methods[(METHOD_OFFSET)]));              \
          goto finish_searching;                                               \
        }                                                                      \
      }                                                                        \
      vm_throw((MACHINE)->state,                                               \
               VM_ERROR_FAIL_TO_FIND_CLASS_IN_VIRTUAL_TABLE);                  \
      (METHOD_INFO) = NULL;                                                    \
    finish_searching:;                                                         \
    }                                                                          \
  }

#endif // VM_MACHINE_H