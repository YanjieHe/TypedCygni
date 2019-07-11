#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "DataStructures.h"

int parse_program(const char* path);

int parse_constant_pool(FILE* file, Program* program);
#endif /* PARSER_H */