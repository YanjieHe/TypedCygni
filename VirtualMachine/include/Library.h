#ifndef VM_LIBRARY_H
#define VM_LIBRARY_H
#include "Data.h"

#ifdef _WIN32
#include <windows.h>
#undef DUPLICATE
#endif

FunctionPointer load_library_function(Machine* machine, const char* library_path, const char* function_name);

#endif // VM_LIBRARY_H