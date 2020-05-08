#ifndef PARSER_H
#define PARSER_H

#include "Data.h"
#include <stdio.h>
#include <stdbool.h>

char* parse_string(FILE* file);

uint16_t parse_ushort(FILE* file);

uint32_t parse_uint(ByteCode* byteCode);

TypeTag parse_type_tag(ByteCode* byteCode);

OpCode parse_opcode(ByteCode* byteCode);

Executable* parse_file(const char* path);

ClassInfo* parse_class(FILE* file);

ModuleInfo* parse_module(FILE* file);

Function* parse_function(FILE* file);

void view_exe(Executable* exe);
void view_function(Function* function);

static const char* op_info[][3] = {
	{ "PUSH_INT_1BYTE", "false", "1" },
	{ "PUSH_INT_2BYTE", "false", "2" },
	{ "PUSH_INT", "false", "2" },

	{ "PUSH_DOUBLE_0", "false", "0" },
	{ "PUSH_DOUBLE_1", "false", "0" },
	{ "PUSH_DOUBLE", "false", "2" },
	{ "PUSH_NULL", "false", "0" },

	{ "PUSH_STACK", "true", "2" },
	{ "POP_STACK", "true", "2" },
	{ "PUSH_CONSTANT", "true", "2" },
	{ "POP_CONSTANT", "true", "2" },
	{ "PUSH_ARRAY", "true", "2" },
	{ "POP_ARRAY", "true", "2" },
	{ "PUSH_FIELD", "true", "2" },
	{ "POP_FIELD", "true", "2" },
	{ "ADD", "true", "0" },
	{ "SUB", "true", "0" },
	{ "MUL", "true", "0" },
	{ "DIV", "true", "0" },
	{ "MOD", "true", "0" },
	{ "BIT_AND", "true", "0" },
	{ "BIT_OR", "true", "0" },
	{ "BIT_XOR", "true", "0" },
	{ "MINUS", "true", "0" },
	{ "BIT_NOT", "true", "0" },
	{ "CAST", "true", "1" },
	{ "UP_CAST", "true", "2" },
	{ "DOWN_CAST", "true", "2" },
	{ "EQ", "true", "0" },
	{ "NE", "true", "0" },
	{ "GT", "true", "0" },
	{ "LT", "true", "0" },
	{ "GE", "true", "0" },
	{ "LE", "true", "0" },
	{ "LOGICAL_AND", "false", "0" },
	{ "LOGICAL_OR", "false", "0" },
	{ "LOGICAL_NOT", "false", "0" },
	{ "POP", "false", "0" },
	{ "DUPLICATE", "false", "0" },
	{ "DUPLICATE_OFFSET", "false", "2" },
	{ "JUMP", "false", "2" },
	{ "JUMP_IF_TRUE", "false", "2" },
	{ "JUMP_IF_FALSE", "false", "2" },
	{ "PUSH_MODULE", "false", "2" },
	{ "PUSH_FUNCTION", "false", "2" },
	{ "PUSH_METHOD", "false", "2" },
	{ "INVOKE", "false", "0" },
	{ "RETURN", "true", "0" },
	{ "NEW", "false", "2" },
	{ "SUPER", "false", "0" },
	{ "INSTANCE_OF", "false", "0" },
	{ "THROW", "false", "0" },
	{ "RETHROW", "false", "0" },
	{ "GO_FINALLY", "false", "0" },
	{ "FINALLY_END", "false", "0" }
};
#endif // PARSER_H