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
	Constant* constant_pool;
	Value* stack;
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
	Object* gc_obj;
	Function* prev_func;
	Function* cur_func;
	Function* next_func;
	void* pointer;
	int stack_index;
	int32_t array_index;

	machine->function = machine->exe->entry;
	cur_func = machine->function;
	code = machine->function->u.func_info->code;
	constant_pool = machine->function->u.func_info->constant_pool;
	stack = machine->stack;
	pc = 0;
	fp = 0;

	// arguments ... (fp) | local variables ... | previous function | last pc | last fp
	stack_index = fp + cur_func->u.func_info->args_size + cur_func->u.func_info->locals;
	stack[stack_index + 1].u.i32_v = pc;
	stack[stack_index + 2].u.i32_v = fp;
	sp = stack_index + 3;

	while (pc < cur_func->u.func_info->code_len)
	{
		op = code[pc];
		printf("function: %s, op = %s\n", cur_func->name, opcode_info[op][0]);
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
					   //case PUSH_STRING: {
					   //	READ_USHORT(index);
					   //	break;
					   //}
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
			gc_obj = machine->exe->modules[index].variables[offset].u.gc_obj;
			STACK_WRITE(gc_obj);
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
			STACK_READ(gc_obj);
			machine->exe->modules[index].variables[offset].u.gc_obj = gc_obj;
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
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			stack[sp].u.i32_v = gc_obj->u.fields[offset].u.i32_v;
			break;
		}
		case PUSH_FIELD_I64: {
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			stack[sp].u.i64_v = gc_obj->u.fields[offset].u.i64_v;
			break;
		}
		case PUSH_FIELD_F32: {
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			stack[sp].u.f32_v = gc_obj->u.fields[offset].u.f32_v;
			break;
		}
		case PUSH_FIELD_F64: {
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			stack[sp].u.f64_v = gc_obj->u.fields[offset].u.f64_v;
			break;
		}
		case POP_FIELD_I32: {
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			gc_obj->u.fields[offset].u.i32_v = stack[sp - 1].u.i32_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_I64: {
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			gc_obj->u.fields[offset].u.i64_v = stack[sp - 1].u.i64_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_F32: {
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			gc_obj->u.fields[offset].u.f32_v = stack[sp - 1].u.f32_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_F64: {
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			gc_obj->u.fields[offset].u.f64_v = stack[sp - 1].u.f64_v;
			sp = sp - 2;
			break;
		}
		case PUSH_ARRAY_I32: {
			STACK_READ(gc_obj);
			STACK_READ(i32_v);
			gc_obj = stack[sp].u.gc_obj;
			i32_v = gc_obj->u.array->u.i32_array[i32_v];
			STACK_WRITE(i32_v);
			break;
		}
		case PUSH_ARRAY_I64: {
			STACK_READ(gc_obj);
			STACK_READ(i32_v);
			gc_obj = stack[sp].u.gc_obj;
			i64_v = gc_obj->u.array->u.i64_array[i32_v];
			STACK_WRITE(i64_v);
			break;
		}
		case PUSH_ARRAY_F32: {
			STACK_READ(gc_obj);
			STACK_READ(i32_v);
			gc_obj = stack[sp].u.gc_obj;
			f32_v = gc_obj->u.array->u.f32_array[i32_v];
			STACK_WRITE(f32_v);
			break;
		}
		case PUSH_ARRAY_F64: {
			STACK_READ(gc_obj);
			STACK_READ(i32_v);
			gc_obj = stack[sp].u.gc_obj;
			f64_v = gc_obj->u.array->u.f64_array[i32_v];
			STACK_WRITE(f64_v);
			break;
		}
		case PUSH_ARRAY_OBJECT: {
			STACK_READ(gc_obj);
			STACK_READ(i32_v);
			gc_obj = stack[sp].u.gc_obj;
			gc_obj = gc_obj->u.array->u.obj_array[i32_v];
			STACK_WRITE(gc_obj);
			break;
		}
		case POP_ARRAY_I32: {
			STACK_READ(i32_v);
			array_index = i32_v;
			STACK_READ(gc_obj); // array
			STACK_READ(i32_v); // value
			gc_obj->u.array->u.i32_array[array_index] = i32_v;
			break;
		}
		case POP_ARRAY_I64: {
			STACK_READ(i32_v);
			array_index = i32_v;
			STACK_READ(gc_obj); // array
			STACK_READ(i64_v); // value
			gc_obj->u.array->u.i64_array[array_index] = i64_v;
			break;
		}
		case POP_ARRAY_F32: {
			STACK_READ(i32_v);
			array_index = i32_v;
			STACK_READ(gc_obj);
			STACK_READ(f32_v);
			gc_obj->u.array->u.f32_array[array_index] = f32_v;
			break;
		}
		case POP_ARRAY_F64: {
			STACK_READ(i32_v);
			array_index = i32_v;
			STACK_READ(gc_obj);
			STACK_READ(f64_v);
			gc_obj->u.array->u.f64_array[array_index] = f64_v;
			break;
		}
		case POP_ARRAY_OBJECT: {
			STACK_READ(i32_v);
			array_index = i32_v;
			STACK_READ(gc_obj);
			gc_obj->u.array->u.obj_array[array_index] = stack[sp].u.gc_obj;
			sp--;
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
			stack_index = fp + cur_func->u.func_info->args_size + cur_func->u.func_info->locals;
			prev_func = stack[stack_index].u.function;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;
			cur_func = prev_func;

			stack[sp].u.i32_v = i32_v;

			break;
		}
		case RETURN_I64: {
			STACK_READ(i64_v);
			stack_index = fp + cur_func->u.func_info->args_size + cur_func->u.func_info->locals;
			prev_func = stack[stack_index].u.function;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;
			cur_func = prev_func;

			stack[sp].u.i64_v = i64_v;
			break;
		}
		case RETURN_F32: {
			STACK_READ(f32_v);
			stack_index = fp + cur_func->u.func_info->args_size + cur_func->u.func_info->locals;
			prev_func = stack[stack_index].u.function;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;
			cur_func = prev_func;

			stack[sp].u.f32_v = f32_v;
			break;
		}
		case RETURN_F64: {
			STACK_READ(f64_v);
			stack_index = fp + cur_func->u.func_info->args_size + cur_func->u.func_info->locals;
			prev_func = stack[stack_index].u.function;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;
			cur_func = prev_func;

			stack[sp].u.f64_v = f64_v;
			break;
		}
		case RETURN_OBJECT: {
			STACK_READ(gc_obj);
			stack_index = fp + cur_func->u.func_info->args_size + cur_func->u.func_info->locals;
			prev_func = stack[stack_index].u.function;
			sp = fp;
			pc = stack[stack_index + 1].u.i32_v;
			fp = stack[stack_index + 2].u.i32_v;
			code = prev_func->u.func_info->code;
			constant_pool = prev_func->u.func_info->constant_pool;
			cur_func = prev_func;

			stack[sp].u.gc_obj = gc_obj;
			break;
		}
		case INVOKE: {
			next_func = stack[sp].u.function;
			if (next_func->is_native_function)
			{
				if (!(next_func->u.native_function->is_loaded))
				{
					next_func->u.native_function->function_pointer = load_library_function(
						next_func->u.native_function->lib_path,
						next_func->u.native_function->func_name);
					next_func->u.native_function->is_loaded = true;
				}
				sp = sp - next_func->u.native_function->args_size;
				next_func->u.native_function->function_pointer(&(stack[sp])); // return value omitted
			}
			else
			{
				int currentFp = fp;
				fp = sp - next_func->u.func_info->args_size;
				stack_index = fp + next_func->u.func_info->args_size + next_func->u.func_info->locals;
				stack[stack_index].u.function = cur_func;
				stack[stack_index + 1].u.i32_v = pc;
				stack[stack_index + 2].u.i32_v = currentFp;
				cur_func = next_func;
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
			stack[sp].u.function = machine->exe->modules[index].functions[offset];
			break;
		}
		case PUSH_METHOD: {
			gc_obj = stack[sp].u.gc_obj;
			READ_USHORT(offset);
			stack[sp].u.function = machine->exe->classes[gc_obj->class_index].methods[offset];
			break;
		}
		case NEW: {
			READ_USHORT(index);
			gc_obj = (Object*)malloc(sizeof(Object));
			gc_obj->is_array = false;
			gc_obj->class_index = index;
			gc_obj->u.fields = (Value*)malloc(sizeof(Value) * machine->exe->classes[index].n_fields);
			gc_obj->next = NULL; // TO DO: garbage collection
			STACK_WRITE(gc_obj);
			stack[sp].is_gc_obj = true;
			break;
		}
		case NEW_ARRAY: {
			switch (code[pc])
			{
			case TYPE_I32: {
				pc = pc + 1;
				STACK_READ(i32_v);
				gc_obj = (Object*)malloc(sizeof(Object));
				gc_obj->is_array = true;
				gc_obj->class_index = 0;
				gc_obj->next = NULL;
				gc_obj->u.array = (Array*)malloc(sizeof(Array));
				gc_obj->u.array->length = i32_v;
				gc_obj->u.array->u.i32_array = malloc(sizeof(int32_t) * i32_v);
				gc_obj->u.array->type = ARRAY_TYPE_I32;
				STACK_WRITE(gc_obj);
				break;
			}
			case TYPE_I64: {
				pc = pc + 1;
				STACK_READ(i32_v);
				gc_obj = (Object*)malloc(sizeof(Object));
				gc_obj->is_array = true;
				gc_obj->class_index = 0;
				gc_obj->next = NULL;
				gc_obj->u.array = (Array*)malloc(sizeof(Array));
				gc_obj->u.array->length = i32_v;
				gc_obj->u.array->u.i64_array = malloc(sizeof(int64_t) * i32_v);
				gc_obj->u.array->type = ARRAY_TYPE_I64;
				STACK_WRITE(gc_obj);
				break;
			}
			case TYPE_F32: {
				pc = pc + 1;
				STACK_READ(i32_v);
				gc_obj = (Object*)malloc(sizeof(Object));
				gc_obj->is_array = true;
				gc_obj->class_index = 0;
				gc_obj->next = NULL;
				gc_obj->u.array = (Array*)malloc(sizeof(Array));
				gc_obj->u.array->length = i32_v;
				gc_obj->u.array->u.f32_array = malloc(sizeof(float_t) * i32_v);
				gc_obj->u.array->type = ARRAY_TYPE_F32;
				STACK_WRITE(gc_obj);
				break;
			}
			case TYPE_F64: {
				pc = pc + 1;
				STACK_READ(i32_v);
				gc_obj = (Object*)malloc(sizeof(Object));
				gc_obj->is_array = true;
				gc_obj->class_index = 0;
				gc_obj->next = NULL;
				gc_obj->u.array = (Array*)malloc(sizeof(Array));
				gc_obj->u.array->length = i32_v;
				gc_obj->u.array->u.i64_array = malloc(sizeof(double_t) * i32_v);
				gc_obj->u.array->type = ARRAY_TYPE_F64;
				STACK_WRITE(gc_obj);
				break;
			}
			case TYPE_OBJECT: {
				pc = pc + 1;
				STACK_READ(i32_v);
				gc_obj = (Object*)malloc(sizeof(Object));
				gc_obj->is_array = true;
				gc_obj->class_index = 0;
				gc_obj->next = NULL;
				gc_obj->u.array = (Array*)malloc(sizeof(Array));
				gc_obj->u.array->length = i32_v;
				gc_obj->u.array->u.obj_array = malloc(sizeof(Object) * i32_v);
				gc_obj->u.array->type = ARRAY_TYPE_OBJECT;
				STACK_WRITE(gc_obj);
				break;
			}
			default:
			{
				fprintf(stderr, "wrong array type\n");
				exit(-1);
			}
			}
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
	printf("try to load function '%s' from library '%s'\n", function_name, library_path);

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

void view_stack(Value* stack)
{
	for (int i = 0; i < 100; i++)
	{
		printf("%d ", stack[i].u.i32_v);
	}
	printf("\n");
}