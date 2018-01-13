#include "ByteCodeGenerator.h"
#include "Exception.h"
#include <iostream>
#include <memory>
using namespace std;

ByteCodeGenerator::ByteCodeGenerator(DebugInfo& debugInfo,
                                     LocationRecord& record)
    : code{&globalCode}, debugInfo{debugInfo}, record{record}
{
}

void ByteCodeGenerator::Visit(UnaryExpression* node)
{
	node->operand->Accept(this);
	if (node->kind == ExpressionKind::UnaryPlus)
	{
		return; // eliminate unary plus
	}
    else if (node->kind == ExpressionKind::Negate && node->type.IsInt())
    {
        Emit(OpCode::minus_i32);
    }
    else if (node->kind == ExpressionKind::Negate && node->type.IsDouble())
    {
        Emit(OpCode::minus_f64);
    }
    else if (node->kind == ExpressionKind::Not && node->type.IsBoolean())
    {
        Emit(OpCode::logical_not);
	}
	else
	{
        throw CompilationException(debugInfo.Locate(node),
                                   L"unary operation: " +
                                       node->type.ToString());
	}
}

void ByteCodeGenerator::Visit(BinaryExpression* node)
{
	node->left->Accept(this);
	node->right->Accept(this);
    bool areInts = node->left->type.IsInt() && node->right->type.IsInt();
    bool areDoubles =
        node->left->type.IsDouble() && node->right->type.IsDouble();
    ExpressionKind kind = node->kind;
    if (kind == ExpressionKind::Add && areInts)
    {
        Emit(OpCode::add_i32);
    }
    else if (kind == ExpressionKind::Add && areDoubles)
    {
        Emit(OpCode::add_f64);
    }
    else if (kind == ExpressionKind::Subtract && areInts)
    {
        Emit(OpCode::sub_i32);
    }
    else if (kind == ExpressionKind::Subtract && areDoubles)
    {
        Emit(OpCode::sub_f64);
    }
    else if (kind == ExpressionKind::Multiply && areInts)
    {
        Emit(OpCode::mul_i32);
    }
    else if (kind == ExpressionKind::Multiply && areDoubles)
    {
        Emit(OpCode::mul_f64);
    }
    else if (kind == ExpressionKind::Divide && areInts)
    {
        Emit(OpCode::div_i32);
    }
    else if (kind == ExpressionKind::Divide && areDoubles)
    {
        Emit(OpCode::div_f64);
    }
    else if (kind == ExpressionKind::Modulo && areInts)
    {
        Emit(OpCode::mod_i32);
    }
    else if (kind == ExpressionKind::Modulo && areDoubles)
    {
        Emit(OpCode::mod_f64);
    }
    else if (kind == ExpressionKind::GreaterThan && areInts)
    {
        Emit(OpCode::gt_i32);
    }
    else if (kind == ExpressionKind::GreaterThan && areDoubles)
    {
        Emit(OpCode::gt_f64);
    }
    else if (kind == ExpressionKind::LessThan && areInts)
    {
        Emit(OpCode::lt_i32);
    }
    else if (kind == ExpressionKind::LessThan && areDoubles)
    {
        Emit(OpCode::lt_i32);
    }
    else if (kind == ExpressionKind::GreaterThanOrEqual && areInts)
    {
        Emit(OpCode::ge_i32);
    }
    else if (kind == ExpressionKind::GreaterThanOrEqual && areDoubles)
    {
        Emit(OpCode::ge_f64);
    }
    else if (kind == ExpressionKind::LessThanOrEqual && areInts)
    {
        Emit(OpCode::le_i32);
    }
    else if (kind == ExpressionKind::LessThanOrEqual && areDoubles)
    {
        Emit(OpCode::le_f64);
    }
    else if (kind == ExpressionKind::Equal && areInts)
    {
        Emit(OpCode::eq_i32);
    }
    else if (kind == ExpressionKind::Equal && areDoubles)
    {
        Emit(OpCode::eq_f64);
    }
    else if (kind == ExpressionKind::NotEqual && areInts)
    {
        Emit(OpCode::ne_i32);
    }
    else if (kind == ExpressionKind::NotEqual && areDoubles)
    {
        Emit(OpCode::ne_f64);
    }
    else
    {
        throw CompilationException(debugInfo.Locate(node),
                                   L"binary operation: " +
                                       expression_kind_to_wstring(node->kind));
    }
}

void ByteCodeGenerator::Visit(ConstantExpression* node)
{
    if (node->type.IsInt())
	{
		int value = node->constant.GetInt();
		const int BYTE_MAX = 256;
		const int USHORT_MAX = 65536;
		if (value >= 0 && value < BYTE_MAX)
		{
			Emit(OpCode::push_i32_1byte);
            code->push_back(static_cast<byte>(value));
		}
		else if (value >= 0 && value < USHORT_MAX)
		{
			Emit(OpCode::push_i32_1byte);
            AppendUShort(static_cast<u16>(value));
		}
		else
		{
			throw L"14 not implemented";
		}
	}
    else if (node->type.IsDouble())
	{
        double value = node->constant.GetDouble();
        if (value == 0.0)
        {
            Emit(OpCode::push_f64_0);
        }
        else if (value == 1.0)
        {
            Emit(OpCode::push_f64_1);
        }
        else
        {
            throw NotImplementedException();
        }
	}
	else
	{
		throw L"16 not implemented";
	}
}

void ByteCodeGenerator::Visit(BlockExpression* node)
{
    for (ExpressionPtr& expression : node->expressions)
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
void ByteCodeGenerator::Visit(ConditionalExpression* node)
{
	node->test->Accept(this);

	Emit(OpCode::jump_if_false);
	int index = CurrentIndex();
    AppendUShort(0);

	node->ifTrue->Accept(this);

    WriteUShort(index, static_cast<u16>(CurrentIndex()));
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
void ByteCodeGenerator::Visit(FullConditionalExpression* node)
{
	node->test->Accept(this);

	Emit(OpCode::jump_if_false);
	int index = CurrentIndex();
    AppendUShort(0);

	node->ifTrue->Accept(this);

	Emit(OpCode::jump);
	int index2 = CurrentIndex();
    AppendUShort(0);

    WriteUShort(index, static_cast<u16>(CurrentIndex()));

	node->ifFalse->Accept(this);
    WriteUShort(index2, static_cast<u16>(CurrentIndex()));
}

void ByteCodeGenerator::Visit(ParameterExpression* node)
{
	Location location = record.Find(node);
    if (node->type.IsInt() && location.kind == LocationKind::Global)
    {
        Emit(OpCode::push_static_i32);
        AppendUShort(static_cast<u16>(location.index));
	}
    else if (node->type.IsInt() && location.kind == LocationKind::InFunction)
	{
        Emit(OpCode::push_stack_i32);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsDouble() && location.kind == LocationKind::Global)
    {
        Emit(OpCode::push_static_f64);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsDouble() && location.kind == LocationKind::InFunction)
    {
        Emit(OpCode::push_stack_f64);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsFunction() &&
             location.kind == LocationKind::FunctionID)
    {
        Emit(OpCode::push_function);
        AppendUShort(static_cast<u16>(location.index));
    }
    else
    {
        throw CompilationException(debugInfo.Locate(node),
                                   L"cannot compile parameter expression: " +
                                       node->name);
	}
}

void ByteCodeGenerator::Visit(CallExpression* node)
{
    for (ExpressionPtr& argument : node->arguments)
	{
		argument->Accept(this);
	}
	node->procedure->Accept(this);
	Emit(OpCode::invoke);
    //    AppendUShort(static_cast<u16>(node->arguments.size()));
}

/*
 * while a < 10
 * {
 * }
 */
void ByteCodeGenerator::Visit(WhileExpression* node)
{
	int index = CurrentIndex();
	node->condition->Accept(this);
	Emit(OpCode::jump_if_false);

	int index2 = CurrentIndex();
    AppendUShort(0);
	node->body->Accept(this);
	Emit(OpCode::jump);

    AppendUShort(static_cast<u16>(index));
    WriteUShort(index2, static_cast<u16>(CurrentIndex()));
}

void ByteCodeGenerator::Visit(VarExpression* node)
{
	node->value->Accept(this);

	Location location = record.Find(node);
    if (node->type.IsInt() && location.kind == LocationKind::Global)
    {
        Emit(OpCode::pop_static_i32);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsInt() && location.kind == LocationKind::InFunction)
    {
        Emit(OpCode::pop_stack_i32);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsDouble() && location.kind == LocationKind::Global)
    {
        Emit(OpCode::pop_static_f64);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsDouble() && location.kind == LocationKind::InFunction)
    {
        Emit(OpCode::pop_stack_f64);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsFunction() &&
             location.kind == LocationKind::FunctionID)
    {
        Emit(OpCode::push_function);
        AppendUShort(static_cast<u16>(location.index));
    }
	else
	{
		throw L"19 not implemented";
	}
}

void ByteCodeGenerator::Visit(AssignExpression* node)
{
    ParameterExpressionPtr variable = node->variable;
    ExpressionPtr exp = static_pointer_cast<Expression>(variable);
    Location location = record.Find(exp);
	node->value->Accept(this);

    if (node->type.IsInt() && location.kind == LocationKind::Global)
    {
        Emit(OpCode::pop_static_i32);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsInt() && location.kind == LocationKind::InFunction)
    {
        Emit(OpCode::pop_stack_i32);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsDouble() && location.kind == LocationKind::Global)
    {
        Emit(OpCode::pop_static_f64);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsDouble() && location.kind == LocationKind::InFunction)
    {
        Emit(OpCode::pop_stack_f64);
        AppendUShort(static_cast<u16>(location.index));
    }
    else if (node->type.IsFunction() &&
             location.kind == LocationKind::FunctionID)
    {
        Emit(OpCode::push_function);
        AppendUShort(static_cast<u16>(location.index));
    }
	else
	{
		throw L"1 not implemented";
	}
}

void ByteCodeGenerator::Visit(DefaultExpression* node)
{
    if (node->type.IsInt())
    {
        Emit(OpCode::push_i32_1byte);
        code->push_back(0);
    }
    else if (node->type.IsDouble())
    {
        Emit(OpCode::push_f64_0);
    }
    else
    {
        throw L"20 not implemented";
    }
}

void ByteCodeGenerator::Visit(DefineExpression* node)
{
    vector<byte> fcode;
	vector<byte>* prev = code;
    code = &fcode;
    node->body->Accept(this);
    functions.push_back(Function(node->name,
                                 static_cast<int>(node->parameters.size()),
                                 node->frameSize, fcode));
	code = prev;
}

void ByteCodeGenerator::Visit(NewExpression*)
{
	throw L"22 not implemented";
}

void ByteCodeGenerator::Visit(ReturnExpression* node)
{
	node->value->Accept(this);
    if (node->type.IsInt())
	{
		Emit(OpCode::return_i32);
	}
    else if (node->type.IsDouble())
	{
		Emit(OpCode::return_f64);
	}
	else
	{
		throw L"return value type not supprted";
    }
}

void ByteCodeGenerator::Visit(ImportExpression*)
{
    return;
}

void ByteCodeGenerator::Emit(OpCode op)
{
	wcout << code->size() << L" emit: " << opcode_to_wstring(op) << endl;
    code->push_back(static_cast<byte>(op));
}

i32 ByteCodeGenerator::CurrentIndex()
{
    return static_cast<i32>(code->size());
}

void ByteCodeGenerator::AppendBytes(byte* bytes, int length)
{
	for (int i = 0; i < length; i++)
	{
		code->push_back(bytes[i]);
	}
}

void ByteCodeGenerator::AppendBytes(ByteCode& code, byte* bytes, int length)
{
    for (int i = 0; i < length; i++)
    {
        code.push_back(bytes[i]);
    }
}

void ByteCodeGenerator::AppendUShort(ByteCode& code, u16 x)
{
    AppendBytes(code, reinterpret_cast<byte*>(&x), sizeof(u16));
}

void ByteCodeGenerator::AppendUShort(u16 x)
{

    AppendBytes(reinterpret_cast<byte*>(&x), sizeof(u16));
}

void ByteCodeGenerator::AppendInt(ByteCode& code, i32 x)
{
    AppendBytes(code, reinterpret_cast<byte*>(&x), sizeof(i32));
}

void ByteCodeGenerator::AppendWChar(ByteCode& code, wchar_t c)
{
    AppendInt(code, static_cast<i32>(c));
}

void ByteCodeGenerator::AppendWString(ByteCode& code, wstring& str)
{

    AppendInt(code, static_cast<i32>(str.size()));
    for (u32 i = 0; i < str.size(); i++)
    {
        AppendWChar(code, str.at(i));
    }
}

void ByteCodeGenerator::WriteBytes(int offset, byte* bytes, int length)
{
	for (int i = 0; i < length; i++)
	{
        code->at(static_cast<u32>(offset + i)) = bytes[i];
	}
}

void ByteCodeGenerator::WriteUShort(int offset, u16 x)
{
    WriteBytes(offset, reinterpret_cast<byte*>(&x), sizeof(u16));
}

ByteCode ByteCodeGenerator::GetTopCode()
{
    ByteCode result;
    for (byte item : *code)
    {
        result.push_back(item);
    }
    return result;
}

ByteCode ByteCodeGenerator::GetFunctionCode(Function& f)
{
    ByteCode result;
    byte fbegin = static_cast<byte>(OpCode::function_begin);
    result.push_back(fbegin);
    AppendWString(result, f.name);
    AppendUShort(result, static_cast<u16>(f.parameterSize));
    AppendUShort(result, static_cast<u16>(f.frameSize));
    for (byte item : f.code)
    {
        result.push_back(item);
    }
    byte fend = static_cast<byte>(OpCode::function_end);
    result.push_back(fend);
    return result;
}
