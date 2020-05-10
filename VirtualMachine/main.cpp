#include "Machine.h"
#include <stdlib.h>
#include "Parser.h"
int main()
{
	const char* path;
	Executable* exe;
	Machine* machine;

	path = "C:\\Users\\HeYan\\CMakeBuilds\\dc11c05f-03b5-4f3a-963f-0c8941d4a6da\\build\\x64-Debug (default)\\sample_code\\app";

	exe = parse_file(path);

	printf("\n");
	view_exe(exe);

	machine = create_machine(1000, exe);
	run(machine);
	getchar();
}