#include "Parser.h"
#include <malloc.h>
#define SUCCESS 0
#define ERROR_READ_I32 -1
#define ERROR_READ_U8 -2
#define ERROR_NOT_IMPLEMENTED -3
#define ERROR_IO -4
#define ERROR_READ_STRING -5
#define ERROR_READ_U32 -6
#define ERROR_READ_BYTES -7
#define ERROR_READ_U16 -8
int parse_program(const char* path, Program* program) {
  FILE* file = fopen(path, "r");
  if (file) {
    int result;
    if ((result = parse_constant_pool(file, program)) == SUCCESS) {
      if (!fread(&program->modules.count, sizeof(int32_t), 1, file) ||
          !(program->modules.count >= 0)) {
        return ERROR_READ_I32;
      }
      if (!fread(&program->classes.count, sizeof(int32_t), 1, file) ||
          !(program->classes.count >= 0)) {
        return ERROR_READ_I32;
      }

      fclose(file);
      return SUCCESS;
    } else {
      fclose(file);
      return result;
    }
  } else {
    return ERROR_IO;
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
  program->constantPool.values =
      malloc(sizeof(TaggedValue) * program->constantPool.count);
  for (int i = 0; i < program->constantPool.count; i++) {
    uint8_t flag;
    if (!fread(&flag, sizeof(uint8_t), 1, file)) {
      return ERROR_READ_U8;
    }
    switch (flag) {
      case CONSTANT_TYPE_INT32: {
        if (!fread(&program->constantPool.values[i].u.i32_v, sizeof(int32_t), 1,
                   file)) {
          return ERROR_READ_I32;
        }
        break;
      }
      default:
        return ERROR_NOT_IMPLEMENTED;
    }
  }
  return SUCCESS;
}

int parse_modules(FILE* file, Program* program, int module_count) {
  for (int i = 0; i < module_count; i++) {
    Module* module = &program->modules.modules[i];
    // TO DO
  }
}

int parse_functions(FILE* file, Module* module, int function_count) {
  int res;
  for (int i = 0; i < function_count; i++) {
    Function* function = &module->functions.functions[i];
    if ((res = parse_string(file, &function->name)) != SUCCESS) {
      return res;
    }
    if (!fread(&function->locals, sizeof(uint16_t), 1, file)) {
      return ERROR_READ_U16;
    }
    if (!fread(&function->stack, sizeof(uint16_t), 1, file)) {
      return ERROR_READ_U16;
    }
    if (!fread(&function->args_size, sizeof(uint16_t), 1, file)) {
      return ERROR_READ_U16;
    }
    if (!fread(&function->code.length, sizeof(uint16_t), 1, file)) {
      return ERROR_READ_U16;
    }
    if (!fread(function->code.bytes, sizeof(Byte), function->code.length,
               file)) {
      return ERROR_READ_BYTES;
    }
  }
}

int parse_string(FILE* file, ByteArray* string) {
  if (!fread(&string->length, sizeof(int32_t), 1, file) ||
      !(string->length >= 0)) {
    return ERROR_READ_STRING;
  }
  if (!fread(string->bytes, sizeof(Byte), string->length, file)) {
    return ERROR_READ_STRING;
  }
  return SUCCESS;
}