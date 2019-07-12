#include <malloc.h>
#include "Parser.h"
#include "View.h"

int main(int argc, char** argv) {
  Program* program = (Program*)malloc(sizeof(Program));
  const char* path =
      "/home/yanjie/Documents/GitHub/TypedCygni/CygniCompiler/"
      "cmake-build-debug/test_output/compiled-code.exe";

  int result = parse_program(path, program);
  if (result == 0) {
    view_program(program);
  } else {
    printf("error code: %d\n", result);
  }
  return 0;
}