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
    int res;
    if ((res = parse_constant_pool(file, program)) != SUCCESS) {
      fclose(file);
      return res;
    }
    if (!fread(&program->modules.count, sizeof(uint16_t), 1, file) ||
        !(program->modules.count >= 0)) {
      return ERROR_READ_I32;
    }
    if (!fread(&program->classes.count, sizeof(uint16_t), 1, file) ||
        !(program->classes.count >= 0)) {
      return ERROR_READ_I32;
    }

    printf("module count = %d\n", program->modules.count);
    program->modules.modules = malloc(sizeof(Module) * program->modules.count);
    if ((res = parse_modules(file, program, program->modules.count)) !=
        SUCCESS) {
      fclose(file);
      return res;
    }

    fclose(file);
    return SUCCESS;
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
  int res;
  for (int i = 0; i < module_count; i++) {
    Module* module = &program->modules.modules[i];
    if ((res = parse_string(file, &module->name)) != SUCCESS) {
      return res;
    }
    print_name("module name = ", &module->name);
    if (!fread(&module->variables.count, sizeof(uint16_t), 1, file)) {
      return ERROR_READ_U16;
    }
    if (!fread(&module->functions.count, sizeof(uint16_t), 1, file)) {
      return ERROR_READ_U16;
    }
    printf("function count = %d\n", module->functions.count);
    module->functions.functions =
        malloc(sizeof(Function) * module->functions.count);
    if ((res = parse_functions(file, module, module->functions.count)) !=
        SUCCESS) {
      return res;
    }
  }
  return SUCCESS;
}

int parse_functions(FILE* file, Module* module, int function_count) {
  int res;
  for (int i = 0; i < function_count; i++) {
    Function* function = &module->functions.functions[i];
    if ((res = parse_string(file, &function->name)) != SUCCESS) {
      return res;
    }
    print_name("function name = ", &function->name);
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
    printf("locals = %d, stack = %d, args_size = %d\n", function->locals,
           function->stack, function->args_size);
    printf("function code length = %d\n", function->code.length);
    function->code.bytes = malloc(sizeof(Byte) * function->code.length);
    if (!fread(function->code.bytes, sizeof(Byte), function->code.length,
               file)) {
      return ERROR_READ_BYTES;
    }
  }
  return SUCCESS;
}

int parse_string(FILE* file, Utf8String* string) {
  if (!fread(&string->length, sizeof(int32_t), 1, file) ||
      !(string->length >= 0)) {
    return ERROR_READ_STRING;
  }
  printf("string length = %d\n", string->length);
  string->bytes = malloc(sizeof(Byte) * string->length);
  if (!fread(string->bytes, sizeof(Byte), string->length, file)) {
    return ERROR_READ_STRING;
  }
  return SUCCESS;
}

void print_name(const char* prompt, Utf8String* string) {
  printf(prompt);
  for (int k = 0; k < string->length; k++) {
    printf("%c", string->bytes[k]);
  }
  printf("\n");
}