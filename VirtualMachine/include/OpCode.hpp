#ifndef OP_CODE_HPP
#define OP_CODE_HPP
#include <string>
#include <iostream>

typedef enum
{
	PUSH_INT_1BYTE,
	PUSH_INT_2BYTE,
	PUSH_INT,

	PUSH_DOUBLE_0,
	PUSH_DOUBLE_1,
	PUSH_DOUBLE,
	PUSH_NULL,

	PUSH_STACK,
	POP_STACK,
	PUSH_CONSTANT,
	POP_CONSTANT,
	PUSH_ARRAY,
	POP_ARRAY,
	PUSH_FIELD,
	POP_FIELD,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	BIT_AND,
	BIT_OR,
	BIT_XOR,
	MINUS,
	BIT_NOT,
	CAST,
	UP_CAST,
	DOWN_CAST,
	EQ,
	NE,
	GT,
	LT,
	GE,
	LE,
	LOGICAL_AND,
	LOGICAL_OR,
	LOGICAL_NOT,
	POP,
	DUPLICATE,
	DUPLICATE_OFFSET,
	JUMP,
	JUMP_IF_TRUE,
	JUMP_IF_FALSE,
	PUSH_MODULE,
	PUSH_FUNCTION,
	INVOKE,
	RETURN,
	NEW,
	SUPER,
	INSTANCE_OF,
	THROW,
	RETHROW,
	GO_FINALLY,
	FINALLY_END
} OpCode;

typedef enum
{
	TYPE_I32,
	TYPE_I64,
	TYPE_F32,
	TYPE_F64,
	TYPE_STRING,
	TYPE_OBJECT
} TypeTag;

static std::string OpCodeToString(OpCode code)
{
	switch (code)
	{
	case PUSH_INT_1BYTE:
		return "PUSH_INT_1BYTE";
	case PUSH_INT_2BYTE:
		return "PUSH_INT_2BYTE";
	case PUSH_INT:
		return "PUSH_INT";
	case PUSH_DOUBLE_0:
		return "PUSH_DOUBLE_0";
	case PUSH_DOUBLE_1:
		return "PUSH_DOUBLE_1";
	case PUSH_DOUBLE:
		return "PUSH_DOUBLE";
	case PUSH_NULL:
		return "PUSH_NULL";
	case PUSH_STACK:
		return "PUSH_STACK";
	case POP_STACK:
		return "POP_STACK";
	case PUSH_CONSTANT:
		return "PUSH_CONSTANT";
	case POP_CONSTANT:
		return "POP_CONSTANT";
	case PUSH_ARRAY:
		return "PUSH_ARRAY";
	case POP_ARRAY:
		return "POP_ARRAY";
	case PUSH_FIELD:
		return "PUSH_FIELD";
	case POP_FIELD:
		return "POP_FIELD";
	case ADD:
		return "ADD";
	case SUB:
		return "SUB";
	case MUL:
		return "MUL";
	case DIV:
		return "DIV";
	case MOD:
		return "MOD";
	case BIT_AND:
		return "BIT_AND";
	case BIT_OR:
		return "BIT_OR";
	case BIT_XOR:
		return "BIT_XOR";
	case MINUS:
		return "MINUS";
	case BIT_NOT:
		return "BIT_NOT";
	case CAST:
		return "CAST";
	case UP_CAST:
		return "UP_CAST";
	case DOWN_CAST:
		return "DOWN_CAST";
	case EQ:
		return "EQ";
	case NE:
		return "NE";
	case GT:
		return "GT";
	case LT:
		return "LT";
	case GE:
		return "GE";
	case LE:
		return "LE";
	case LOGICAL_AND:
		return "LOGICAL_AND";
	case LOGICAL_OR:
		return "LOGICAL_OR";
	case LOGICAL_NOT:
		return "LOGICAL_NOT";
	case POP:
		return "POP";
	case DUPLICATE:
		return "DUPLICATE";
	case DUPLICATE_OFFSET:
		return "DUPLICATE_OFFSET";
	case JUMP:
		return "JUMP";
	case JUMP_IF_TRUE:
		return "JUMP_IF_TRUE";
	case JUMP_IF_FALSE:
		return "JUMP_IF_FALSE";
	case PUSH_MODULE:
		return "PUSH_MODULE";
	case PUSH_FUNCTION:
		return "PUSH_FUNCTION";
	case INVOKE:
		return "INVOKE";
	case RETURN:
		return "RETURN";
	case NEW:
		return "NEW";
	case SUPER:
		return "SUPER";
	case INSTANCE_OF:
		return "INSTANCE_OF";
	case THROW:
		return "THROW";
	case RETHROW:
		return "RETHROW";
	case GO_FINALLY:
		return "GO_FINALLY";
	case FINALLY_END:
		return "FINALLY_END";
	default:
		std::cout << __FUNCTION__ << std::endl;
		exit(1);
	}
}

static std::string TypeTagToString(TypeTag tag)
{
	switch (tag)
	{
	case TYPE_I32:
		return "TYPE_I32";
	case TYPE_I64:
		return "TYPE_I64";
	case TYPE_F32:
		return "TYPE_F32";
	case TYPE_F64:
		return "TYPE_F64";
	case TYPE_STRING:
		return "TYPE_STRING";
	case TYPE_OBJECT:
		return "TYPE_OBJECT";
	default:
		std::cout << __FUNCTION__ << std::endl;
		exit(1);
	}
}
#endif // OP_CODE_HPP