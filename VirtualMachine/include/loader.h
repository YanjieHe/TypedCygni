#ifndef VM_PARSER_H
#define VM_PARSER_H
#include "cygni.h"
#include "state.h"
#include <stdio.h>
#include "executable.h"
Byte* read_byte_code_from_file(char* path, int* file_length);

char* parse_string(State* state);

Byte parse_byte(State* state);

uint16_t parse_ushort(State* state);

Executable* parse_file(State* state, const char* path);

void parse_class(State* state, ClassInfo* class_info);

//void parse_module(State* state, ModuleInfo* module_info);

void parse_method(State* state, MethodInfo* function, ClassInfo* class_info);

void parse_constant_pool(State* state, ConstantPool* constant_pool);

void view_exe(Executable* exe);

void view_function(MethodInfo* function, Executable* exe);

void view_type_tag(State* state, Byte tag);

static bool expect_length(State* state, i32 length)
{
	return state->byte_code.offset + length <= state->byte_code.size;
}

bool is_initializer(char* full_qualified_name);
#endif // VM_PARSER_H