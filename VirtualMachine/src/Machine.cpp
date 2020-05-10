#include "Machine.h"
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
	Value* constantPool;
	Value* stack;
	Function* function;
	int pc;
	int fp;
	int sp;
	Byte op;
	Byte type;
	uint16_t offset;
	uint16_t index;
	int32_t i32_v;
	int64_t i64_v;
	float_t f32_v;
	double_t f64_v;
	Object* obj;
	Function* prev_func;
	Function* next_func;
	void* pointer;

	machine->function = machine->exe->entry;
	function = machine->function;
	code = machine->function->code;
	constantPool = machine->function->constantPool;
	stack = machine->stack;
	pc = 0;
	fp = 0;

	// arguments ... (fp) | local variables ... | last function | last pc | last fp
	index = fp + function->n_parameters + function->locals + 1;
	pc = stack[index + 1].u.i32_v;
	fp = stack[index + 2].u.i32_v;
	sp = index + 3;

	while (true)
	{
		op = code[pc];
		pc + 1;
		switch (op)
		{
		case PUSH_I32: {
			READ_USHORT(index);
			i32_v = constantPool[index].u.i32_v;
			STACK_WRITE(i32_v);
			break;
		}
		case PUSH_I64: {
			READ_USHORT(index);
			i64_v = constantPool[index].u.i64_v;
			STACK_WRITE(i64_v);
			break;
		}
		case PUSH_F32: {
			READ_USHORT(index);
			f32_v = constantPool[index].u.f32_v;
			STACK_WRITE(f32_v);
			break;
		}
		case PUSH_F64: {
			READ_USHORT(index);
			f64_v = constantPool[index].u.f64_v;
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
			stack[sp].u.i32_v = obj->fields[offset].u.i32_v;
			break;
		}
		case PUSH_FIELD_I64: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			stack[sp].u.i64_v = obj->fields[offset].u.i64_v;
			break;
		}
		case PUSH_FIELD_F32: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			stack[sp].u.f32_v = obj->fields[offset].u.f32_v;
			break;
		}
		case PUSH_FIELD_F64: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			stack[sp].u.f64_v = obj->fields[offset].u.f64_v;
			break;
		}
		case POP_FIELD_I32: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			obj->fields[offset].u.i32_v = stack[sp - 1].u.i32_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_I64: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			obj->fields[offset].u.i64_v = stack[sp - 1].u.i64_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_F32: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			obj->fields[offset].u.f32_v = stack[sp - 1].u.f32_v;
			sp = sp - 2;
			break;
		}
		case POP_FIELD_F64: {
			obj = (Object*)stack[sp].u.obj;
			READ_USHORT(offset);
			obj->fields[offset].u.f64_v = stack[sp - 1].u.f64_v;
			sp = sp - 2;
			break;
		}
		case JUMP: {
			pc = USHORT(code, pc + 1);
			break;
		}
		case JUMP_IF_TRUE: {
			STACK_READ(i32_v);
			if (i32_v)
			{
				pc = USHORT(code, pc + 1);
			}
			else
			{
				pc = pc + 1 + 2;
			}
			break;
		}
		case JUMP_IF_FALSE: {
			STACK_READ(i32_v);
			if (i32_v)
			{
				pc = pc + 1 + 2;
			}
			else
			{
				pc = USHORT(code, pc + 1);
			}
			break;
		}
		case RETURN_I32: {
			STACK_READ(i32_v);
			index = fp + function->n_parameters + function->locals + 1;
			prev_func = (Function*)stack[index].u.obj;
			sp = fp;
			pc = stack[index + 1].u.i32_v;
			fp = stack[index + 2].u.i32_v;
			code = prev_func->code;
			constantPool = prev_func->constantPool;

			stack[sp].u.i32_v = i32_v;
			break;
		}
		case RETURN_I64: {
			STACK_READ(i64_v);
			index = fp + function->n_parameters + function->locals + 1;
			prev_func = (Function*)stack[index].u.obj;
			sp = fp;
			pc = stack[index + 1].u.i32_v;
			fp = stack[index + 2].u.i32_v;
			code = prev_func->code;
			constantPool = prev_func->constantPool;

			stack[sp].u.i64_v = i64_v;
			break;
		}
		case RETURN_F32: {
			STACK_READ(f32_v);
			index = fp + function->n_parameters + function->locals + 1;
			prev_func = (Function*)stack[index].u.obj;
			sp = fp;
			pc = stack[index + 1].u.i32_v;
			fp = stack[index + 2].u.i32_v;
			code = prev_func->code;
			constantPool = prev_func->constantPool;

			stack[sp].u.f32_v = f32_v;
			break;
		}
		case RETURN_F64: {
			STACK_READ(f64_v);
			index = fp + function->n_parameters + function->locals + 1;
			prev_func = (Function*)stack[index].u.obj;
			sp = fp;
			pc = stack[index + 1].u.i32_v;
			fp = stack[index + 2].u.i32_v;
			code = prev_func->code;
			constantPool = prev_func->constantPool;

			stack[sp].u.f64_v = f64_v;
			break;
		}
		case RETURN_OBJECT: {
			pointer = stack[sp].u.obj;
			sp--;
			index = fp + function->n_parameters + function->locals + 1;
			prev_func = (Function*)stack[index].u.obj;
			sp = fp;
			pc = stack[index + 1].u.i32_v;
			fp = stack[index + 2].u.i32_v;
			code = prev_func->code;
			constantPool = prev_func->constantPool;

			stack[sp].u.obj = pointer;
			break;
		}
		case INVOKE: {
			next_func = (Function*)stack[sp].u.obj;
			sp--;
			int currentPc = pc;
			int currentFp = fp;
			fp = sp - next_func->n_parameters;
			int index = fp + next_func->n_parameters + next_func->locals + 1;
			stack[index].u.obj = function;
			stack[index + 1].u.i32_v = currentPc;
			stack[index + 2].u.i32_v = currentFp;
			function = next_func;
			pc = 0;
			sp = index + 3;
			code = next_func->code;
			constantPool = next_func->constantPool;
			break;
		}
		case PUSH_FUNCTION: {
			READ_USHORT(index);
			READ_USHORT(offset);
			sp++;
			stack[sp].u.obj = machine->exe->modules[index]->functions[offset];
			break;
		}
		case PUSH_METHOD: {
			READ_USHORT(index);
			READ_USHORT(offset);
			sp++;
			stack[sp].u.obj = machine->exe->classes[index]->methods[offset];
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
