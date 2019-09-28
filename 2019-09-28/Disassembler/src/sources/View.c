#include "View.h"
#include <stdio.h>
#include <stdlib.h>

void view_program(Program* program) {
  view_constant_pool(&program->constantPool);
}

void view_constant_pool(ConstantPool* constantPool) {
  enum ConstantType {
    CONSTANT_TYPE_INT32,
    CONSTANT_TYPE_INT64,
    CONSTANT_TYPE_FLOAT32,
    CONSTANT_TYPE_FLOAT64,
    CONSTANT_TYPE_BOOL,
    CONSTANT_TYPE_CHAR,
    CONSTANT_TYPE_STRING
  };
  printf("constant pool size = %d\n", constantPool->count);
  for (int i = 0; i < constantPool->count; i++) {
    switch (constantPool->values[i].tag) {
      case CONSTANT_TYPE_INT32:
        printf("#%d(i32) = %d\n", i, constantPool->values[i].u.i32_v);
        break;

      default:
        printf("FUCK\n");
        exit(0);
        break;
    }
  }
}