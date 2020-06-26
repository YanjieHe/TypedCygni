#include "library.h"
#include "machine.h"

FunctionPointer load_library_function(State* state, const char * library_path, const char * entry_point)
{
	HINSTANCE lib;
	FunctionPointer function_pointer;

	printf("try to load function '%s' from library '%s'\n", entry_point, library_path);
	
	lib = LoadLibrary(library_path);

	if (lib)
	{
		function_pointer = (FunctionPointer)GetProcAddress(lib, entry_point);
		if (function_pointer)
		{
			return function_pointer;
		}
		else
		{
			fprintf(stderr, "cannot load function '%s' in the library: %s\n", entry_point, library_path);
			fprintf(stderr, "error code: %d", GetLastError());
			vm_throw(state, VM_ERROR_LOAD_LIBRARY_FUNCTION);
			return NULL; // make the compiler happy
		}
	}
	else
	{
		fprintf(stderr, "cannot load library: %s\n", library_path);
		fprintf(stderr, "error code: %d", GetLastError());
		vm_throw(state, VM_ERROR_LOAD_LIBRARY);
		return NULL; // make the compiler happy
	}
}