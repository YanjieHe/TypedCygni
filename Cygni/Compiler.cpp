#include "Compiler.h"
#include "Exception.h"
#include <iostream>

using namespace std;

Compiler::Compiler(DebugInfo& debugInfo, LocationRecord& record)
	:debugInfo{debugInfo}, record{record}
{
	code = new vector<byte>();
}

void Compiler::Visit(UnaryExpression* node)
{
	node->operand->Accept(this);
	if (node->kind == ExpressionKind::UnaryPlus)
	{
		return; // eliminate unary plus
	}
	else if (node->kind == ExpressionKind::Negate)
	{
		if (node->GetType()->IsInt())
		{
			Emit(OpCode::minus_i32);
		}
		else if (node->GetType()->IsDouble())
		{
			Emit(OpCode::minus_f64);
		}
		else
		{
			throw CompilationException(debugInfo.Locate(node),
					L"negate: " + node->GetType()->ToString());
		}
	}
	else if (node->kind == ExpressionKind::Not)
	{
		if (node->GetType()->IsBoolean())
		{
			throw L"23 not implemented";
		}
		else
		{
			throw L"6 impossible";
		}
	}
	else
	{
		throw L"7 impossible";
	}
}

void Compiler::Visit(BinaryExpression* node)
{
	node->left->Accept(this);
	node->right->Accept(this);

	switch (node->kind)
	{
		case ExpressionKind::Add:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::add_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::add_f64);
			}
			else
			{
				throw L"2 not implemented";
			}
			break;
		}
		case ExpressionKind::Subtract:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::sub_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::sub_f64);
			}
			else
			{
				throw L"3 not implemented";
			}
			break;
		}
		case ExpressionKind::Multiply:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::mul_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::mul_f64);
			}
			else
			{
				throw L"4 not implemented";
			}
			break;
		}
		case ExpressionKind::Divide:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::div_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::div_f64);
			}
			else
			{
				throw L"5 not implemented";
			}
			break;
		}
		case ExpressionKind::Modulo:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::mod_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::mod_f64);
			}
			else
			{
				throw L"6 not implemented";
			}
			break;
		}

		case ExpressionKind::GreaterThan:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::gt_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::gt_f64);
			}
			else
			{
				throw L"7 not implemented";
			}
			break;
		}
		case ExpressionKind::LessThan:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::lt_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::lt_f64);
			}
			else
			{
				throw L"8 not implemented";
			}
			break;
		}
		case ExpressionKind::GreaterThanOrEqual:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::ge_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::ge_f64);
			}
			else
			{
				throw L"9 not implemented";
			}
			break;
		}
		case ExpressionKind::LessThanOrEqual:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::lt_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::lt_f64);
			}
			else
			{
				throw L"10 not implemented";
			}
			break;
		}
		case ExpressionKind::Equal:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::eq_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::eq_i32);
			}
			else
			{
				throw L"11 not implemented";
			}
			break;
		}
		case ExpressionKind::NotEqual:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				Emit(OpCode::ne_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				Emit(OpCode::ne_f64);
			}
			else
			{
				throw L"12 not implemented";
			}
			break;
		}
		default:
		{
			wcout << expression_kind_to_wstring(node->kind) << endl;	
			throw L"13 not implemented";
		}
	}
}

void Compiler::Visit(ConstantExpression* node)
{
	if (node->GetType()->IsInt())
	{
		int value = node->constant.GetInt();
		const int BYTE_MAX = 256;
		const int USHORT_MAX = 65536;
		if (value >= 0 && value < BYTE_MAX)
		{
			Emit(OpCode::push_i32_1byte);
			code->push_back((byte) value);
		}
		else if (value >= 0 && value < USHORT_MAX)
		{
			Emit(OpCode::push_i32_1byte);
			AppendUShort((unsigned short) value);
		}
		else
		{
			throw L"14 not implemented";
		}
	}
	else if (node->GetType()->IsDouble())
	{
		throw L"15 not implemented";
	}
	else
	{
		throw L"16 not implemented";
	}
}

void Compiler::Visit(BlockExpression* node)
{
	for (Expression* expression: node->expressions)
	{
		expression->Accept(this);
	}
}

/*
 * if a == 0
 * {
 * 	   a = 10
 * }
 *
 * bytecode:
 * 0: push a
 * 3: push 0
 * 6: eq_int
 * 7: jump_if_false 15
 * 10: push 10
 * 12: pop_static a
 * 15: 
 */
void Compiler::Visit(ConditionalExpression* node)
{
	node->test->Accept(this);

	Emit(OpCode::jump_if_false);
	int index = CurrentIndex();
	AppendUShort((unsigned short)0);

	node->ifTrue->Accept(this);

	WriteUShort(index, (unsigned short) CurrentIndex());
}

/*
 * if a == 0
 * {
 * 	   a = 10
 * }
 * else
 * {
 *     a = 15
 * }
 *
 * bytecode:
 * 0: push a
 * 3: push 0
 * 6: eq_int
 * 7: jump_if_false 18
 * 10: push 10
 * 12: pop_static a
 * 15: jump 24
 * 18: push 15
 * 21: pop_static a
 * 24: 
 */
void Compiler::Visit(FullConditionalExpression* node)
{
	node->test->Accept(this);

	Emit(OpCode::jump_if_false);
	int index = CurrentIndex();
	AppendUShort((unsigned short)0);

	node->ifTrue->Accept(this);
	
	Emit(OpCode::jump);
	int index2 = CurrentIndex();
	AppendUShort((unsigned short)0);

	WriteUShort(index, (unsigned short) CurrentIndex());

	node->ifFalse->Accept(this);
	wcout << "write " << CurrentIndex() << endl;
	WriteUShort(index2, (unsigned short) CurrentIndex());
}

void Compiler::Visit(ParameterExpression* node)
{
	Location location = record.Find(node);
	if (node->GetType()->IsInt())
	{
		if (location.kind == LocationKind::Global)
		{
			Emit(OpCode::push_static_i32);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			Emit(OpCode::push_stack_i32);
			AppendUShort(location.index);
		}
		else
		{
			throw L"1 impossible";
		}
	}
	else if (node->GetType()->IsDouble())
	{
		if (location.kind == LocationKind::Global)
		{
			Emit(OpCode::push_static_f64);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			Emit(OpCode::push_stack_f64);
			AppendUShort(location.index);
		}
		else
		{
			throw L"2 impossible";
		}
	}
	else if (node->GetType()->tag == TypeTag::Function)
	{
		if (location.kind == LocationKind::StaticMethod)
		{
			Emit(OpCode::push_function);
			AppendUShort(location.index);
		}
		else
		{
			throw L"cannot compile procedure";
		}
	}
	else
	{
		throw L"17 not implemented";
	}
}

void Compiler::Visit(CallExpression* node)
{
	for (Expression* argument: node->arguments)
	{
		argument->Accept(this);
	}
	node->procedure->Accept(this);
	Emit(OpCode::invoke);
	AppendUShort((unsigned short) node->arguments.size());
}

/*
 * while a < 10
 * {
 * }
 */
void Compiler::Visit(WhileExpression* node)
{
	int index = CurrentIndex();
	node->condition->Accept(this);
	Emit(OpCode::jump_if_false);
	int index2 = CurrentIndex();
	AppendUShort((unsigned short)0);
	node->body->Accept(this);
	Emit(OpCode::jump);
	AppendUShort(index);
	WriteUShort(index2, (unsigned short) CurrentIndex());
}

void Compiler::Visit(VarExpression* node)
{
	node->value->Accept(this);

	Location location = record.Find(node);
	if (node->GetType()->IsInt())
	{
		if (location.kind == LocationKind::Global)
		{
			Emit(OpCode::pop_static_i32);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			Emit(OpCode::pop_stack_i32);
			AppendUShort(location.index);
		}
		else
		{
			throw L"3 impossible";
		}
	}
	else if (node->GetType()->IsDouble())
	{
		if (location.kind == LocationKind::Global)
		{
			Emit(OpCode::pop_static_f64);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			Emit(OpCode::pop_stack_f64);
			AppendUShort(location.index);
		}
		else
		{
			throw L"4 impossible";
		}
	}
	else
	{
		throw L"19 not implemented";
	}
}

void Compiler::Visit(AssignExpression* node)
{
	ParameterExpression* variable = node->variable;
	Location location = record.Find(variable);
	node->value->Accept(this);

	if (node->GetType()->IsInt())
	{
		if (location.kind == LocationKind::Global)
		{
			Emit(OpCode::pop_static_i32);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			Emit(OpCode::pop_stack_i32);
			AppendUShort(location.index);
		}
		else
		{
			throw L"8 impossible";
		}
	}
	else if (node->GetType()->IsDouble())
	{
		if (location.kind == LocationKind::Global)
		{
			Emit(OpCode::pop_static_f64);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			Emit(OpCode::pop_stack_f64);
			AppendUShort(location.index);
		}
		else
		{
			throw L"9 impossible";
		}
	}
	else if (node->GetType()->tag == TypeTag::Function)
	{
		if (location.kind == LocationKind::StaticMethod)
		{
			Emit(OpCode::push_function);
			AppendUShort(location.index);
		}
		else
		{
			throw L"parameter is not static method";
		}
	}
	else
	{
		throw L"1 not implemented";
	}
}

void Compiler::Visit(DefaultExpression* node)
{
	throw L"20 not implemented";
}

void Compiler::Visit(DefineExpression* node)
{
	vector<byte>* fcode = new vector<byte>();
	vector<byte>* prev = code;
	code = fcode;
	//Emit(OpCode::function_begin);
	//AppendUShort((unsigned short) node->parameters.size());
	//AppendUShort((unsigned short) node->frameSize);
	node->body->Accept(this);
	// Emit(OpCode::function_end);
	functions.push_back(new Function(node->name, node->parameters.size(), node->frameSize, fcode));
	code = prev;
}

void Compiler::Visit(NewExpression* node)
{
	throw L"22 not implemented";
}

void Compiler::Visit(ReturnExpression* node)
{
	node->value->Accept(this);
	if (node->GetType()->IsInt())
	{
		Emit(OpCode::return_i32);
	}
	else if (node->GetType()->IsDouble())
	{
		Emit(OpCode::return_f64);
	}
	else
	{
		throw L"return value type not supprted";
	}
}

void Compiler::Emit(OpCode op)
{
	wcout << code->size() << L" emit: " << opcode_to_wstring(op) << endl;
	code->push_back((byte) op);
}

i32 Compiler::CurrentIndex()
{
	return code->size();
}

void Compiler::AppendBytes(byte* bytes, int length)
{
	for (int i = 0; i < length; i++)
	{
		code->push_back(bytes[i]);
	}
}

void Compiler::AppendUShort(unsigned short x)
{
	AppendBytes((byte*)&x, 2);
}

void Compiler::WriteBytes(int offset, byte* bytes, int length)
{
	for (int i = 0; i < length; i++)
	{
		code->at(offset + i) = bytes[i];
	}
}

void Compiler::WriteUShort(int offset, unsigned short x)
{
	WriteBytes(offset, (byte*)&x, 2);
}
