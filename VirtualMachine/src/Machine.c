#include "Machine.h"
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define STACK_WRITE(VALUE) sp++; stack[sp].u.##VALUE = (VALUE);
#define STACK_READ(VALUE) (VALUE) = stack[sp].u.##VALUE; sp--;

#define READ_USHORT(VALUE) VALUE = (((uint16_t)code[pc]) << 8) + ((uint16_t)code[pc + 1]); pc = pc + 2;

Machine * create_machine(int stack_max_size, Executable * exe)
{
	Machine * machine;

	machine = (Machine*)malloc(sizeof(Machine));
	machine->stack_max_size = stack_max_size;
	machine->stack = (Value*)malloc(sizeof(Value) * stack_max_size);
	machine->exe = exe;
	machine->fp = -1;
	machine->sp = -1;
	machine->pc = 0;
	machine->function = NULL;

	return machine;
}

void run(Machine* machine)
{
	Byte* code;
	Value* constant_pool;
	Value* stack;
	Function* function;
	int pc;
	int fp;
	int sp;
	Byte op;
	uint16_t offset;
	uint16_t index;
	int32_t i32_v;
	int64_t i64_v;
	float_t f32_v;
	double_t f64_v;
	void* obj;
	Object* typed_obj;
	Function* prev_func;
	Function* next_func;
	void* pointer;
	int stack_index;

	machine->function = machine->exe->entry;
	function = machine->function;
	code = machine->function->u.func_info->code;
	constant_pool = machine->function->u.func_info->constant_pool;
	stack = machine->stack;
	pc = 0;
	fp = 0;

	// arguments ... (fp) | local variables ... | previous function | last pc | last fp
	stack_index = fp + function->u.func_info->n_parameters + function->u.func_info->locals + 1;
	stack[stack_index + 1].u.i32_v = pc;
	stack[stack_index + 2].u.i32_v = fp;
	sp = stack_index + 3;

	while (pc < function->u.func_info->code_len)
	{
		printf("%s, pc = %d\n", function->name, pc);
		op = code[pc];
		pc = pc + 1;
		switch (op)
		{
		case PUSH_I32_0: {
			sp++;
			stack[sp].u.i32_v = 0;
			break;
		}
		case PUSH_I32_1: {
			sp++;
			stack[sp].u.i32_v = 1;
			break;
		}
		case PUSH_I32: {
			READ_USHORT(index);
			i32_v = constant_pool[index].u.i32_v;
			STACK_WRITE(i32_v);
			break;
		}
		case PUSH_I64: {
			READ_USHORT(index);
			i64_v = constant_pool[index].u.i64_v;
			STACK_WRITE(i64_v);
			break;
		}
		case PUSH_F32: {
			READ_USHORT(index);
			f32_v = constant_pool[index].u.f32_v;
			STACK_WRITE(f32_v);
			break;
		}
		case PUSH_F64: {
			READ_USHORT(index);
			f64_v = constant_pool[index].u.f64_v;
			STACK_WRITE(f64_v);
			break;
		}
		case PUSH_LOCAL_I32: {
			READ_USHORT(offset);
			i32_v = stack[fp + offset].u.i32_v;
			STACK_WRITE(i32_v);
			break;
		}
		case PUSH_LOCAL_I64: {
			READ_USHORT(offset);
			i64_v = stack[fp + offset].u.i64_v;
			STACK_WRITE(i64_v);
			break;
		}
		case PUSH_LOCAL_F32: {
			READ_USHORT(offset);
			f32_v = stack[fp + offset].u.f32_v;
			STACK_WRITE(f32_v);
			break;
		}
		case PUSH_LOCAL_F64: {
			READ_USHORT(offset);
			f64_v = stack[fp + offset].u.f64_v;
			STACK_WRITE(f64_v);
			break;
		}
		case POP_LOCAL_I32: {
			READ_USHORT(offset);
			STACK_READ(i32_v);
			stack[fp + offset].u.i32_v = i32_v;
			break;
		}
		case POP_LOCAL_I64: {
			READ_USHORT(offset);
			STACK_READ(i64_v);
			stack[fp + offset].u.i64_v = i64_v;
			break;
		}
		case POP_LOCAL_F32: {
			READ_USHORT(offset);
			STACK_READ(f32_v);
			stack[fp + offset].u.f32_v = f32_v;
			break;
		}
		case POP_LOCAL_F64: {
			READ_USHORT(offset);
			STACK_READ(i64_v);
			stack[fp + offset].u.i64_v = i64_v;
			break;
		}
		case PUSH_STATIC_I32: {
			READ_USHORT(index);
			READ_USHORT(offset);
			i32_v = machine->exe->modules[index].variables[offset].u.i32_v;
			STACK_WRITE(i32_v);
			break;
		}
		case PUSH_STATIC_I64: {
			READ_USHORT(index);
			READ_USHORT(offset);
			i64_v = machine->exe->modules[index].variables[offset].u.i64_v;
			STACK_WRITE(i64_v);
			break;
		}
		case PUSH_STATIC_F32: {
			READ_USHORT(index);
			READ_USHORT(offset);
			f32_v = machine->exe->modules[index].variables[offset].u.f32_v;
			STACK_WRITE(f32_v);
			break;
		}
		case PUSH_STATIC_F64: {
			READ_USHORT(index);
			READ_USHORT(offset);
			f64_v = machine->exe->modules[index].variables[offset].u.f64_v;
			STACK_WRITE(f64_v);
			break;
		}
		case PUSH_STATIC_OBJECT: {
			READ_USHORT(index);
			READ_USHORT(offset);
			obj = machine->exe->modules[index].variables[offset].u.obj;
			STACK_WRITE(obj);
			break;
		}
		case POP_STATIC_I32: {
			READ_USHORT(index);
			READ_USHORT(offset);
			STACK_READ(i32_v);
			machine->exe->modules[index].variables[offset].u.i32_v = i32_v;
			break;
		}
		case POP_STATIC_I64: {
			READ_USHORT(index);
			READ_USHORT(offset);
			STACK_READ(i64_v);
			machine->exe->modules[index].variables[offset].u.i64_v = i64_v;
			break;
		}
		case POP_STATIC_F32: {
			READ_USHORT(index);
			READ_USHORT(offset);
			STACK_READ(f32_v);
			machine->exe->modules[index].variables[offset].u.f32_v = f32_v;
			break;
		}
		case POP_STATIC_F64: {
			READ_USHORT(index);
			READ_USHORT(offset);
			STACK_READ(f64_v);
			machine->exe->modules[index].variables[offset].u.f64_v = f64_v;
			break;
		}
		case POP_STATIC_OBJECT: {
			READ_USHORT(index);
			READ_USHORT(offset);
			STACK_READ(obj);
			machine->exe->modules[index].variables[offset].u.obj = obj;
			break;
		}
		case ADD_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v + stack[sp].u.i32_v);
			sp--;
			break;
		}
		case ADD_I64: {
			stack[sp - 1].u.i64_v = (stack[sp - 1].u.i64_v + stack[sp].u.i64_v);
			sp--;
			break;
		}
		case ADD_F32: {
			stack[sp - 1].u.f32_v = (stack[sp - 1].u.f32_v + stack[sp].u.f32_v);
			sp--;
			break;
		}
		case ADD_F64: {
			stack[sp - 1].u.f64_v = (stack[sp - 1].u.f64_v + stack[sp].u.f64_v);
			sp--;
			break;
		}
		case SUB_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v - stack[sp].u.i32_v);
			sp--;
			break;
		}
		case SUB_I64: {
			stack[sp - 1].u.i64_v = (stack[sp - 1].u.i64_v - stack[sp].u.i64_v);
			sp--;
			break;
		}
		case SUB_F32: {
			stack[sp - 1].u.f32_v = (stack[sp - 1].u.f32_v - stack[sp].u.f32_v);
			sp--;
			break;
		}
		case SUB_F64: {
			stack[sp - 1].u.f64_v = (stack[sp - 1].u.f64_v - stack[sp].u.f64_v);
			sp--;
			break;
		}
		case MUL_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v * stack[sp].u.i32_v);
			sp--;
			break;
		}
		case MUL_I64: {
			stack[sp - 1].u.i64_v = (stack[sp - 1].u.i64_v * stack[sp].u.i64_v);
			sp--;
			break;
		}
		case MUL_F32: {
			stack[sp - 1].u.f32_v = (stack[sp - 1].u.f32_v * stack[sp].u.f32_v);
			sp--;
			break;
		}
		case MUL_F64: {
			stack[sp - 1].u.f64_v = (stack[sp - 1].u.f64_v * stack[sp].u.f64_v);
			sp--;
			break;
		}
		case DIV_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v / stack[sp].u.i32_v);
			sp--;
			break;
		}
		case DIV_I64: {
			stack[sp - 1].u.i64_v = (stack[sp - 1].u.i64_v / stack[sp].u.i64_v);
			sp--;
			break;
		}
		case DIV_F32: {
			stack[sp - 1].u.f32_v = (stack[sp - 1].u.f32_v / stack[sp].u.f32_v);
			sp--;
			break;
		}
		case DIV_F64: {
			stack[sp - 1].u.f64_v = (stack[sp - 1].u.f64_v / stack[sp].u.f64_v);
			sp--;
			break;
		}
		case GT_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v > stack[sp].u.i32_v);
			sp--;
			break;
		}
		case GT_I64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v > stack[sp].u.i64_v);
			sp--;
			break;
		}
		case GT_F32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v > stack[sp].u.f32_v);
			sp--;
			break;
		}
		case GT_F64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v > stack[sp].u.f64_v);
			sp--;
			break;
		}
		case LT_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v < stack[sp].u.i32_v);
			sp--;
			break;
		}
		case LT_I64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v < stack[sp].u.i64_v);
			sp--;
			break;
		}
		case LT_F32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v < stack[sp].u.f32_v);
			sp--;
			break;
		}
		case LT_F64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v < stack[sp].u.f64_v);
			sp--;
			break;
		}
		case GE_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v >= stack[sp].u.i32_v);
			sp--;
			break;
		}
		case GE_I64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v >= stack[sp].u.i64_v);
			sp--;
			break;
		}
		case GE_F32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v >= stack[sp].u.f32_v);
			sp--;
			break;
		}
		case GE_F64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v >= stack[sp].u.f64_v);
			sp--;
			break;
		}
		case LE_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v <= stack[sp].u.i32_v);
			sp--;
			break;
		}
		case LE_I64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v <= stack[sp].u.i64_v);
			sp--;
			break;
		}
		case LE_F32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v <= stack[sp].u.f32_v);
			sp--;
			break;
		}
		case LE_F64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v <= stack[sp].u.f64_v);
			sp--;
			break;
		}
		case EQ_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v == stack[sp].u.i32_v);
			sp--;
			break;
		}
		case EQ_I64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v == stack[sp].u.i64_v);
			sp--;
			break;
		}
		case EQ_F32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v == stack[sp].u.f32_v);
			sp--;
			break;
		}
		case EQ_F64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v == stack[sp].u.f64_v);
			sp--;
			break;
		}
		case NE_I32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i32_v != stack[sp].u.i32_v);
			sp--;
			break;
		}
		case NE_I64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.i64_v != stack[sp].u.i64_v);
			sp--;
			break;
		}
		case NE_F32: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f32_v != stack[sp].u.f32_v);
			sp--;
			break;
		}
		case NE_F64: {
			stack[sp - 1].u.i32_v = (stack[sp - 1].u.f64_v != stack[sp].u.f64_v);
			sp--;
			break;
		}
		case PUSH_FIELD_I32: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			stack[sp].u.i32_v = ((Object*)obj)->fields[offset].u.i32_v;
			break;
		}
		case PUSH_FIELD_I64: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			stack[sp].u.i64_v = ((Object*)obj)->fields[offset].u.i64_v;
			break;
		}
		case PUSH_FIELD_F32: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			stack[sp].u.f32_v = ((Object*)obj)->fields[offset].u.f32_v;
			break;
		}
		case PUSH_FIELD_F64: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			stack[sp].u.f64_v = ((Object*)obj)->fields[offset].u.f64_v;
			break;
		}
		case POP_FIELD_I32: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			((Object*)obj)->fields[offset].u.i32_v = stack[sp - 1].u.i32_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_I64: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			((Object*)obj)->fields[offset].u.i64_v = stack[sp - 1].u.i64_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_F32: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			((Object*)obj)->fields[offset].u.f32_v = stack[sp - 1].u.f32_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_F64: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			((Object*)obj)->fields[offset].u.f64_v = stack[sp - 1].u.f64_v;
			sp = sp - 2;
			break;
		}
		case JUMP: {
			pc = bytes_to_u16(code, pc);
			break;
		}
		case JUMP_IF_TRUE: {
			STACK_READ(i32_v);
			if (i32_v)
			{
				pc = bytes_to_u16(code, pc);
			}
			else
			{
				pc = pc + 2;
			}
			break;
		}
		case JUMP_IF_FALSE: {
			STACK_READ(i32_v);
			if (i32_v)
			{
				pc = pc + 2;
			}
			else
			{
				pc = bytes_to_u16(code, pc);
			}
			break;
		}
		case RETURN_I32: {
			STACK_READ(i32_v);
			stack_index = fp + function->u.func_info->n_parameters + function->u.func_info->locals + 1;
			prev_func = (Function*)stack[stack_index].u.obj;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;

			stack[sp].u.i32_v = i32_v;
			break;
		}
		case RETURN_I64: {
			STACK_READ(i64_v);
			stack_index = fp + function->u.func_info->n_parameters + function->u.func_info->locals + 1;
			prev_func = (Function*)stack[stack_index].u.obj;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;

			stack[sp].u.i64_v = i64_v;
			break;
		}
		case RETURN_F32: {
			STACK_READ(f32_v);
			stack_index = fp + function->u.func_info->n_parameters + function->u.func_info->locals + 1;
			prev_func = (Function*)stack[stack_index].u.obj;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;

			stack[sp].u.f32_v = f32_v;
			break;
		}
		case RETURN_F64: {
			STACK_READ(f64_v);
			stack_index = fp + function->u.func_info->n_parameters + function->u.func_info->locals + 1;
			prev_func = (Function*)stack[stack_index].u.obj;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;

			stack[sp].u.f64_v = f64_v;
			break;
		}
		case RETURN_OBJECT: {
			pointer = stack[sp].u.obj;
			sp--;
			stack_index = fp + function->u.func_info->n_parameters + function->u.func_info->locals + 1;
			prev_func = (Function*)stack[stack_index].u.obj;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;

			stack[sp].u.obj = pointer;
			break;
		}
		case INVOKE: {
			next_func = (Function*)stack[sp].u.obj;
			sp--;
			if (next_func->is_native_function)
			{
				if (!(next_func->u.native_function->is_loaded))
				{
					next_func->u.native_function->function_pointer = load_library_function(
						next_func->u.native_function->lib_path,
						next_func->u.native_function->func_name);
					next_func->u.native_function->is_loaded = true;
				}
				next_func->u.native_function->function_pointer(
					&(stack[sp + 1 - next_func->u.native_function->n_parameters]),
					&(stack[sp + 1 - next_func->u.native_function->n_parameters]));
				sp = sp + 1 - next_func->u.native_function->n_parameters;
			}
			else
			{
				int currentFp = fp;
				fp = sp - next_func->u.func_info->n_parameters;
				stack_index = fp + next_func->u.func_info->n_parameters + next_func->u.func_info->locals + 1;
				stack[stack_index].u.obj = function;
				stack[stack_index + 1].u.i32_v = pc;
				stack[stack_index + 2].u.i32_v = currentFp;
				function = next_func;
				pc = 0;
				sp = stack_index + 3;
				code = next_func->u.func_info->code;
				constant_pool = next_func->u.func_info->constant_pool;
			}
			break;
		}
		case PUSH_FUNCTION: {
			READ_USHORT(index);
			READ_USHORT(offset);
			sp++;
			stack[sp].u.obj = machine->exe->modules[index].functions[offset];
			break;
		}
		case PUSH_METHOD: {
			typed_obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			stack[sp].u.obj = typed_obj->class_info->methods[offset];
			break;
		}
		case NEW: {
			READ_USHORT(index);
			typed_obj = (Object*)malloc(sizeof(Object));
			typed_obj->class_info = &(machine->exe->classes[index]);
			typed_obj->fields = (Value*)malloc(sizeof(Value) * typed_obj->class_info->n_fields);
			typed_obj->next = NULL; // TO DO: garbage collection
			STACK_WRITE(obj);
			stack[sp].is_pointer = true;
			break;
		}
		default: {
			fprintf(stderr, "unsupported operation code: ");
			fprintf(stderr, "%d\n", op);
			exit(-1);
		}
		}
	}
}

FunctionPointer load_library_function(const char * library_path, const char * function_name)
{
	HINSTANCE lib;
	FunctionPointer function_pointer;

	lib = LoadLibrary(library_path);

	if (lib)
	{
		function_pointer = (FunctionPointer)GetProcAddress(lib, "function_pointer");
		if (function_pointer)
		{
			return function_pointer;
		}
		else
		{
			fprintf(stderr, "cannot load function '%s' in the library: %s\n", function_name, library_path);
			exit(-1);
		}
	}
	else
	{
		fprintf(stderr, "cannot load library: %s\n", library_path);
		fprintf(stderr, "error code: %d", GetLastError());
		exit(-1);
	}
}
