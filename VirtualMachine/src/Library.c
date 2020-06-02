#include "Library.h"

FunctionPointer load_library_function(Machine* machine, const char * library_path, const char * function_name)
{
	HINSTANCE lib;
	FunctionPointer function_pointer;

	lib = LoadLibrary(library_path);
	//printf("try to load function '%s' from library '%s'\n", function_name, library_path);

	if (lib)
	{
		function_pointer = (FunctionPointer)GetProcAddress(lib, function_name);
		if (function_pointer)
		{
			return function_pointer;
		}
		else
		{
			fprintf(stderr, "cannot load function '%s' in the library: %s\n", function_name, library_path);
			fprintf(stderr, "error code: %d", GetLastError());
			vm_throw(machine->state, VM_ERROR_LOAD_LIBRARY_FUNCTION);
			return NULL; // make the compiler happy
		}
	}
	else
	{
		fprintf(stderr, "cannot load library: %s\n", library_path);
		fprintf(stderr, "error code: %d", GetLastError());
		vm_throw(machine->state, VM_ERROR_LOAD_LIBRARY);
		return NULL; // make the compiler happy
	}
}