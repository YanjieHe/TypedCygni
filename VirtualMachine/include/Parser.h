#ifndef VM_PARSER_H
#define VM_PARSER_H
#include "Data.h"
#include "Memory.h"
#include <stdio.h>

char* parse_string(State* state);

uint16_t parse_ushort(State* state);

Executable* parse_file(State* state, const char* path);

void parse_class(State* state, ClassInfo* class_info);

void parse_module(State* state, ModuleInfo* module_info);

Function* parse_function(State* state);

void parse_constant_pool(State* state, ConstantPool* constant_pool);

void view_exe(Executable* exe);

void view_function(Function* function);

void view_type_tag(State* state, Byte tag);

#endif // VM_PARSER_H