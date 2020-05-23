#include "Machine.h"
#include <stdlib.h>
#include "Parser.h"
#include "Unicode.h"
#include <stdbool.h>
#include "Memory.h"

int main()
{
	State state;
	const char* path;
	int error_code;

	state.source = NULL;
	state.blocks = NULL;
	state.alloc_ptr = NULL;
	state.mem_size = 0;

	Executable* exe;
	Machine* machine;

	path = "c:\\users\\heyan\\cmakebuilds\\dc11c05f-03b5-4f3a-963f-0c8941d4a6da\\build\\x64-debug (default)\\sample_code\\app";
	printf("parse exe file:\n");
	exe = parse_file(&state, path);
	if ((error_code = setjmp(state.target)) != 0)
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