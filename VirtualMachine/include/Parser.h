#ifndef PARSER_H
#define PARSER_H

#include "Data.h"
#include <stdio.h>

char* parse_string(FILE* file);

uint16_t parse_ushort(FILE* file);

TypeTag parse_type_tag(ByteCode* byteCode);

OpCode parse_opcode(ByteCode* byteCode);

Executable* parse_file(const char* path);

ClassInfo* parse_class(FILE* file);

ModuleInfo* parse_module(FILE* file);

Function* parse_function(FILE* file);

void view_exe(Executable* exe);

void view_function(Function* function);

void view_type_tag(uint8_t tag);

#endif // PARSER_H