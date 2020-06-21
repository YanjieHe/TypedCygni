#include <stdlib.h>
#include <stdbool.h>
#include "machine.h"
#include "memory.h"
#include "unicode.h"
#include "loader.h"

int main()
{
	State* state;
	const char* path;
	int error_code;
	Executable* exe;
	Machine* machine;
	int file_length;
	Byte* byte_code;

	state = state_new();


	path = "c:\\users\\heyan\\cmakebuilds\\dc11c05f-03b5-4f3a-963f-0c8941d4a6da\\build\\x64-debug (default)\\sample_code\\app";
	printf("parse exe file:\n");

	byte_code = read_byte_code_from_file(path, &file_length);

	if (file_length == -1)
	{
		fprintf(stderr, "fail to load executable file: %s\n", path);
		return -1;
	}
	else
	{
		state->byte_code.size = file_length;
		state->byte_code.code = byte_code;

		exe = parse_file(state, path);
		if ((error_code = setjmp(state->target)) != 0)
		{
			fprintf(stderr, "error code: %d\n", error_code);
			fprintf(stderr, "fail to load byte code file\n");
		}
		else
		{
			printf("\nview exe:\n");
			view_exe(exe);
		}

		machine = create_machine(1000, exe);
		run(machine);

		getchar();
	}
}