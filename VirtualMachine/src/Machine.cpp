#include "Machine.hpp"

Machine::Machine(int stackSize, Program * program)
	:stack(stackSize), code{ nullptr }, program{ program },
	constantPool{ nullptr }, function{ nullptr },
	fp{ -1 }, sp{ -1 }, pc{ 0 }
{
}

void Machine::Run()
{
	Function* entry = program->entry;
	function = entry;
	code = &(entry->code);
	constantPool = &(entry->constantPool);
	pc = 0;
	fp = 0;
	int index = fp + function->numArgs + function->locals + 1;
	pc = stack[index + 1].u.i32_v;
	fp = stack[index + 2].u.i32_v;
	sp = stack[index + 3].u.i32_v;
	sp = index + 4;
	while (true) {
		Byte op = (*code)[pc];
		switch (op) {
		case PUSH_CONSTANT: {
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {
				uint16_t index = USHORT(code, pc + 2);
				int32_t value = (*constantPool)[index].u.i32_v;
				sp++;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {
				uint16_t index = USHORT(code, pc + 2);
				int64_t value = (*constantPool)[index].u.i64_v;
				sp++;
				stack[sp].u.i64_v = value;
				break;
			}
			case TYPE_F32: {
				uint16_t index = USHORT(code, pc + 2);
				float_t value = (*constantPool)[index].u.f32_v;
				sp++;
				stack[sp].u.f32_v = value;
				break;
			}
			case TYPE_F64: {
				uint16_t index = USHORT(code, pc + 2);
				double_t value = (*constantPool)[index].u.f64_v;
				sp++;
				stack[sp].u.f64_v = value;
				break;
			}
			}
			pc = pc + 2 + 2;
			break;
		}
		case PUSH_STACK: {
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32:
			{
				uint16_t offset = USHORT(code, pc + 2);
				int32_t value = stack[fp + offset].u.i32_v;
				sp++;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64:
			{
				uint16_t offset = USHORT(code, pc + 2);
				int64_t value = stack[fp + offset].u.i64_v;
				sp++;
				stack[sp].u.i64_v = value;
				break;
			}
			case TYPE_F32:
			{
				uint16_t offset = USHORT(code, pc + 2);
				float_t value = stack[fp + offset].u.f32_v;
				sp++;
				stack[sp].u.f32_v = value;
				break;
			}
			case TYPE_F64:
			{
				uint16_t offset = USHORT(code, pc + 2);
				double_t value = stack[fp + offset].u.f64_v;
				sp++;
				stack[sp].u.f64_v = value;
				break;
			}
			}
			pc = pc + 2 + 2;
			break;
		}
		case POP_STACK: {
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: /* POP_STACK */
			{
				int32_t value = stack[sp].u.i32_v;
				sp--;
				uint16_t offset = USHORT(code, pc + 2);
				stack[fp + offset].u.i32_v = value;
				break;
			}
			case TYPE_I64:/* POP_STACK */
			{
				int64_t value = stack[sp].u.i64_v;
				sp--;
				uint16_t offset = USHORT(code, pc + 2);
				stack[fp + offset].u.i64_v = value;
				break;
			}
			case TYPE_F32:/* POP_STACK */
			{
				float_t value = stack[sp].u.f32_v;
				sp--;
				uint16_t offset = USHORT(code, pc + 2);
				stack[fp + offset].u.f32_v = value;
				break;
			}
			case TYPE_F64:/* POP_STACK */
			{
				double_t value = stack[sp].u.f64_v;
				sp--;
				uint16_t offset = USHORT(code, pc + 2);
				stack[fp + offset].u.f64_v = value;
				break;
			}
			}
			pc = pc + 2 + 2;
			break;
		}
		case ADD:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left + right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int64_t value = left + right;
				stack[sp].u.i64_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				float_t value = left + right;
				stack[sp].u.f32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				double_t value = left + right;
				stack[sp].u.f64_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case SUB:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left - right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int64_t value = left - right;
				stack[sp].u.i64_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				float_t value = left - right;
				stack[sp].u.f32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				double_t value = left - right;
				stack[sp].u.f64_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case MUL:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left * right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int64_t value = left * right;
				stack[sp].u.i64_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				float_t value = left * right;
				stack[sp].u.f32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				double_t value = left * right;
				stack[sp].u.f64_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case DIV:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left / right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int64_t value = left / right;
				stack[sp].u.i64_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				float_t value = left / right;
				stack[sp].u.f32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				double_t value = left / right;
				stack[sp].u.f64_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case GT:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left > right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int32_t value = left > right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				int32_t value = left > right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				int32_t value = left > right;
				stack[sp].u.i32_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case LT:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left < right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int32_t value = left < right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				int32_t value = left < right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				int32_t value = left < right;
				stack[sp].u.i32_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case GE:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left >= right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int32_t value = left >= right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				int32_t value = left >= right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				int32_t value = left >= right;
				stack[sp].u.i32_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case LE:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left <= right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int32_t value = left <= right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				int32_t value = left <= right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				int32_t value = left <= right;
				stack[sp].u.i32_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case EQ:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left == right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int32_t value = left == right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				int32_t value = left == right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				int32_t value = left == right;
				stack[sp].u.i32_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case NE:
		{
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {

				int32_t right = stack[sp].u.i32_v;
				sp--;
				int32_t left = stack[sp].u.i32_v;
				int32_t value = left != right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {

				int64_t right = stack[sp].u.i32_v;
				sp--;
				int64_t left = stack[sp].u.i64_v;
				int32_t value = left != right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F32: {

				float_t right = stack[sp].u.f32_v;
				sp--;
				float_t left = stack[sp].u.f32_v;
				int32_t value = left != right;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_F64: {

				double_t right = stack[sp].u.f64_v;
				sp--;
				double_t left = stack[sp].u.f64_v;
				int32_t value = left != right;
				stack[sp].u.i32_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case PUSH_FIELD: {
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {
				Object* object = (Object*)stack[sp].u.obj;
				uint16_t offset = USHORT(code, pc + 2);
				stack[sp].u.i32_v = object->fields[offset].u.i32_v;
				break;
			}
			case TYPE_I64: {
				Object* object = (Object*)stack[sp].u.obj;
				uint16_t offset = USHORT(code, pc + 2);
				stack[sp].u.i64_v = object->fields[offset].u.i64_v;
				break;
			}
			case TYPE_F32: {
				Object* object = (Object*)stack[sp].u.obj;
				uint16_t offset = USHORT(code, pc + 2);
				stack[sp].u.f32_v = object->fields[offset].u.f32_v;
				break;
			}
			case TYPE_F64: {
				Object* object = (Object*)stack[sp].u.obj;
				uint16_t offset = USHORT(code, pc + 2);
				stack[sp].u.f64_v = object->fields[offset].u.f64_v;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		case POP_FIELD: {
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {
				Object* object = (Object*)stack[sp].u.obj;
				uint16_t offset = USHORT(code, pc + 2);
				sp--;
				object->fields[offset].u.i32_v = stack[sp].u.i32_v;
				sp--;
				break;
			}
			case TYPE_I64: {
				Object* object = (Object*)stack[sp].u.obj;
				uint16_t offset = USHORT(code, pc + 2);
				sp--;
				object->fields[offset].u.i64_v = stack[sp].u.i64_v;
				sp--;
				break;
			}
			case TYPE_F32: {
				Object* object = (Object*)stack[sp].u.obj;
				uint16_t offset = USHORT(code, pc + 2);
				sp--;
				object->fields[offset].u.f32_v = stack[sp].u.f32_v;
				sp--;
				break;
			}
			case TYPE_F64: {
				Object* object = (Object*)stack[sp].u.obj;
				uint16_t offset = USHORT(code, pc + 2);
				sp--;
				object->fields[offset].u.f64_v = stack[sp].u.f64_v;
				sp--;
				break;
			}
			}
			break;
		}
		case JUMP: {
			pc = USHORT(code, pc + 1);
			break;
		}
		case JUMP_IF_TRUE: {
			int32_t value = stack[sp].u.i32_v;
			sp--;
			if (value) {
				pc = USHORT(code, pc + 1);
			}
			else {
				pc = pc + 1 + 2;
			}
			break;
		}
		case JUMP_IF_FALSE: {
			int32_t value = stack[sp].u.i32_v;
			sp--;
			if (value) {
				pc = pc + 1 + 2;
			}
			else {
				pc = USHORT(code, pc + 1);
			}
			break;
		}
		case RETURN: {
			Byte type = ((*code)[pc + 1]);
			switch (type) {
			case TYPE_I32: {
				int32_t value = stack[sp].u.i32_v;
				sp--;
				int index = fp + function->numArgs + function->locals + 1;
				Function* lastFunction = (Function*)stack[index].u.obj;
				pc = stack[index + 1].u.i32_v;
				fp = stack[index + 2].u.i32_v;
				sp = stack[index + 3].u.i32_v;
				stack[sp].u.i32_v = value;
				break;
			}
			case TYPE_I64: {
				int64_t value = stack[sp].u.i64_v;
				sp--;
				int index = fp + function->numArgs + function->locals + 1;
				Function* lastFunction = (Function*)stack[index].u.obj;
				pc = stack[index + 1].u.i32_v;
				fp = stack[index + 2].u.i32_v;
				sp = stack[index + 3].u.i32_v;
				stack[sp].u.i64_v = value;
				break;
			}
			case TYPE_F32: {
				float_t value = stack[sp].u.f32_v;
				sp--;
				int index = fp + function->numArgs + function->locals + 1;
				Function* lastFunction = (Function*)stack[index].u.obj;
				pc = stack[index + 1].u.i32_v;
				fp = stack[index + 2].u.i32_v;
				sp = stack[index + 3].u.i32_v;
				stack[sp].u.f32_v = value;
				break;
			}
			case TYPE_F64: {
				double_t value = stack[sp].u.f64_v;
				sp--;
				int index = fp + function->numArgs + function->locals + 1;
				Function* lastFunction = (Function*)stack[index].u.obj;
				pc = stack[index + 1].u.i32_v;
				fp = stack[index + 2].u.i32_v;
				sp = stack[index + 3].u.i32_v;
				stack[sp].u.f64_v = value;
				break;
			}
			}
			pc = pc + 2;
			break;
		}
		default: {
			std::cout << "unsupported operation code: " << OpCodeToString((OpCode)op) << std::endl;
			exit(1);
		}
		}
	}
}
