#ifndef VM_MACHINE_H
#define VM_MACHINE_H
#include "cygni.h"
#include "memory.h"
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

//int find_virtual_table(ClassInfo *classInfo, int interface_index);
#endif // VM_MACHINE_H