#ifndef PARSER_H
#define PARSER_H
#include "Data.h"
#include <stdio.h>

char* parse_string(FILE* file);

uint16_t parse_ushort(FILE* file);

Executable* parse_file(const char* path);

void parse_class(FILE* file, ClassInfo* class_info);

void parse_module(FILE* file, ModuleInfo* module_info);

Function* parse_function(FILE* file);

void view_exe(Executable* exe);

void view_function(Function* function);

void view_type_tag(uint8_t tag);

#endif // PARSER_H