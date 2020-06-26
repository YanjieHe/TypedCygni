#include "loader.h"
#include "machine.h"
#include "state.h"
#include "unicode.h"
#include <stdbool.h>
#include <stdlib.h>

int main() {
  State *state;
  char *path;
  int error_code;
  Executable *exe;
  Machine *machine;
  int file_length;
  Byte *byte_code;

  state = state_new();

  path = "c:\\users\\heyan\\cmakebuilds\\dc11c05f-03b5-4f3a-963f-"
         "0c8941d4a6da\\build\\x64-debug "
         "(default)\\sample_code\\test2\\build\\app";
  printf("parse exe file:\n");

  byte_code = read_byte_code_from_file(path, &file_length);

  if (file_length == -1) {
    fprintf(stderr, "fail to load executable file: %s\n", path);
    return -1;
  } else {
    state->byte_code.size = file_length;
    state->byte_code.code = byte_code;
    fprintf(stdout, "byte code size = %d\n", file_length);

    exe = parse_file(state, path);
    if ((error_code = setjmp(state->target)) != 0) {
      fprintf(stderr, "error code: %d\n", error_code);
      fprintf(stderr, "fail to load byte code file\n");
      vm_free(state);
      getchar();
      return -1;
    } else {
      printf("\nview exe:\n");
      view_exe(exe);
    }

    fprintf(stdout, "START MACHINE!!!\n");
    machine = create_machine(1000, exe);
    machine->state = state;
    fprintf(stdout, "MACHINE CREATED!!!\n");
    run(machine);
    vm_free(state);
    fprintf(stdout, "PROGRAM EXIT!!!\n");
    getchar();
    return 0;
  }
}