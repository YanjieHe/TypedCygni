#ifndef VM_LIBRARY_H
#define VM_LIBRARY_H
#include "cygni.h"
#include "state.h"

#ifdef _WIN32
#include <windows.h>
#undef DUPLICATE
#endif

FunctionPointer load_library_function(State* state, const char* library_path, const char* entry_point);

#endif // VM_LIBRARY_H