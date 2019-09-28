#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "DataStructures.h"

int parse_program(const char* path, Program* program);

int parse_constant_pool(FILE* file, Program* program);

int parse_modules(FILE* file, Program* program, int module_count);

int parse_functions(FILE* file, Module* module, int function_count);

int parse_string(FILE* file, Utf8String* string);

void print_name(const char* prompt, Utf8String* string);
#endif /* PARSER_H */