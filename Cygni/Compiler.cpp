#include "Compiler.h"
#include "OpCode.h"
#include <iostream>

using namespace std;

Compiler::Compiler(LocationRecord& record)
	:record{record}
{
	code = new vector<byte>();
}

void Compiler::Visit(UnaryExpression* node)
{
	node->operand->Accept(this);
	if (node->kind == ExpressionKind::UnaryPlus)
	{
		return;
	}
	else if (node->kind == ExpressionKind::Negate)
	{
		if (node->GetType()->IsInt())
		{
			code->push_back((byte)OpCode::minus_i32);
		}
		else if (node->GetType()->IsDouble())
		{
			code->push_back((byte)OpCode::minus_f64);
		}
		else
		{
			throw L"5 impossible";
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
	if (node->kind == ExpressionKind::Assign)
	{
		auto variable = (ParameterExpression*) node->left;
		Location location = record.Find(variable);
		node->right->Accept(this);
		if (node->GetType()->IsInt())
		{
			if (location.kind == LocationKind::Global)
			{
				code->push_back((byte)OpCode::pop_static_i32);
				AppendUShort(location.index);
			}
			else if (location.kind == LocationKind::Function)
			{
				code->push_back((byte)OpCode::pop_stack_i32);
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
				code->push_back((byte)OpCode::pop_static_f64);
				AppendUShort(location.index);
			}
			else if (location.kind == LocationKind::Function)
			{
				code->push_back((byte)OpCode::pop_stack_f64);
				AppendUShort(location.index);
			}
			else
			{
				throw L"9 impossible";
			}
		}
		else
		{
			throw L"1 not implemented";
		}
		return;
	}

	node->left->Accept(this);
	node->right->Accept(this);

	switch (node->kind)
	{
		case ExpressionKind::Add:
		{
			if (node->left->GetType()->IsInt() &&
				node->right->GetType()->IsInt())
			{
				code->push_back((byte)OpCode::add_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::add_f64);
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
				code->push_back((byte)OpCode::sub_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::sub_f64);
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
				code->push_back((byte)OpCode::mul_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::mul_f64);
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
				code->push_back((byte)OpCode::div_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::div_f64);
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
				code->push_back((byte)OpCode::mod_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::mod_f64);
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
				code->push_back((byte)OpCode::gt_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::gt_f64);
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
				code->push_back((byte)OpCode::lt_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::lt_f64);
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
				code->push_back((byte)OpCode::ge_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::ge_f64);
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
				code->push_back((byte)OpCode::lt_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::lt_f64);
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
				code->push_back((byte)OpCode::eq_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::eq_i32);
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
				code->push_back((byte)OpCode::ne_i32);
			}
			else if (node->left->GetType()->IsDouble() &&
				node->right->GetType()->IsDouble())
			{
				code->push_back((byte)OpCode::ne_f64);
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
			code->push_back((byte)OpCode::push_i32_1byte);
			code->push_back((byte)value);
		}
		else if (value >= 0 && value < USHORT_MAX)
		{
			code->push_back((byte)OpCode::push_i32_1byte);
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

	code->push_back((byte)OpCode::jump_if_false);
	int index = code->size();
	AppendUShort((unsigned short)0);

	node->ifTrue->Accept(this);

	WriteUShort(index, (unsigned short) code->size());
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

	code->push_back((byte)OpCode::jump_if_false);
	int index = code->size();
	AppendUShort((unsigned short)0);

	node->ifTrue->Accept(this);
	
	code->push_back((byte)OpCode::jump);
	AppendUShort((unsigned short)0);
	int index2 = code->size();
	WriteUShort(index, (unsigned short) code->size());

	node->ifFalse->Accept(this);
	WriteUShort(index2, (unsigned short) code->size());
}

void Compiler::Visit(ParameterExpression* node)
{
	Location location = record.Find(node);
	if (node->GetType()->IsInt())
	{
		if (location.kind == LocationKind::Global)
		{
			code->push_back((byte)OpCode::push_static_i32);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			code->push_back((byte)OpCode::push_stack_i32);
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
			code->push_back((byte)OpCode::push_static_f64);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			code->push_back((byte)OpCode::push_stack_f64);
			AppendUShort(location.index);
		}
		else
		{
			throw L"2 impossible";
		}
	}
	else
	{
		throw L"17 not implemented";
	}
}

void Compiler::Visit(CallExpression* node)
{
	throw L"18 not implemented";
}

/*
 * while a < 10
 * {
 * }
 */
void Compiler::Visit(WhileExpression* node)
{
	int index = code->size();
	node->condition->Accept(this);
	code->push_back((byte)OpCode::jump_if_false);
	int index2 = code->size();
	AppendUShort((unsigned short)0);
	node->body->Accept(this);
	code->push_back((byte)OpCode::jump);
	AppendUShort(index);
	WriteUShort(index2, (unsigned short) code->size());
}

void Compiler::Visit(VarExpression* node)
{
	node->value->Accept(this);

	Location location = record.Find(node);
	wcout << "name = " << node->name << endl;
	wcout << "Location = " << location.ToString() << endl;
	if (node->GetType()->IsInt())
	{
		if (location.kind == LocationKind::Global)
		{
			code->push_back((byte)OpCode::pop_static_i32);
			wcout << L"location index: " << location.index << endl;
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			code->push_back((byte)OpCode::pop_stack_i32);
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
			code->push_back((byte)OpCode::pop_static_f64);
			AppendUShort(location.index);
		}
		else if (location.kind == LocationKind::Function)
		{
			code->push_back((byte)OpCode::pop_stack_f64);
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

void Compiler::Visit(DefaultExpression* node)
{
	throw L"20 not implemented";
}

void Compiler::Visit(DefineExpression* node)
{
	throw L"21 not implemented";
}

void Compiler::Visit(NewExpression* node)
{
	throw L"22 not implemented";
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
