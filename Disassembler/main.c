#include <malloc.h>
#include "Parser.h"
#include "View.h"

int main(int argc, char** argv) {
  Program* program = (Program*)malloc(sizeof(Program));
  const char* path =
      "/home/yanjie/Documents/GitHub/TypedCygni/CygniCompiler/"
      "cmake-build-debug/test_output/compiled-code.exe";

  int32_t v = 3;
  char* bytes = (unsigned char*)&v;
  for (int i = 0; i < sizeof(int32_t); i++) {
    printf("%d\n", bytes[i]);
  }
  int result = parse_program(path, program);
  if (result == 0) {
    view_program(program);
  } else {
    printf("error code: %d\n", result);
  }
  return 0;
}