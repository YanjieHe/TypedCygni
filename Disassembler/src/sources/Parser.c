#include "Parser.h"
#include "malloc.h"
#define SUCCESS 0
#define ERROR_READ_I32 -1
#define ERROR_READ_U8 -2
int parse_program(const char* path) {
  FILE* file = fopen(path, "rb");
  Program* program = malloc(sizeof(Program));
  int result;
  result = parse_constant_pool(file, program);
  if (result == SUCCESS) {
    return SUCCESS;
  } else {
    return result;
  }
}

int parse_constant_pool(FILE* file, Program* program) {
  enum ConstantType {
    CONSTANT_TYPE_INT32,
    CONSTANT_TYPE_INT64,
    CONSTANT_TYPE_FLOAT32,
    CONSTANT_TYPE_FLOAT64,
    CONSTANT_TYPE_BOOL,
    CONSTANT_TYPE_CHAR,
    CONSTANT_TYPE_STRING
  };
  if (!fread(&program->constantPool.count, sizeof(int32_t), 1, file) ||
      !(program->constantPool.count >= 0)) {
    return ERROR_READ_I32;
  }
  for (int i = 0; i < program->constantPool.count; i++) {
    uint8_t flag;
    if (!fread(&flag, sizeof(uint8_t), 1, file)) {
      return ERROR_READ_U8;
    }
    switch(flag){
        case CONSTANT_TYPE_INT32:{
            
            break;
        }
    }
  }
  return SUCCESS;
}