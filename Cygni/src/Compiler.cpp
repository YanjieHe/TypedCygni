#include "Compiler.hpp"
#include "Exception.hpp"

namespace cygni
{
	std::vector<Byte> Compiler::StringToBytes(std::u32string u32str)
	{
		std::vector<Byte> bytes;
		std::string u8str = UTF32ToUTF8(u32str);
		for (Byte c : u8str)
		{
			bytes.push_back(c);
		}
		return bytes;
	}
	void Compiler::Append(std::vector<Byte>& byteCode, Byte byte)
	{
		byteCode.push_back(byte);
	}
	void Compiler::AppendOp(std::vector<Byte>& byteCode, OpCode op)
	{
		byteCode.push_back(static_cast<Byte>(op));
	}
	void Compiler::AppendTypeTag(std::vector<Byte>& byteCode, TypeTag tag)
	{
		byteCode.push_back(static_cast<Byte>(tag));
	}
	void Compiler::AppendUShort(std::vector<Byte>& byteCode, int value)
	{
		byteCode.push_back(value % 256);
		byteCode.push_back(value / 256);
	}
	void Compiler::WriteUShort(std::vector<Byte>& byteCode, int index, int value)
	{
		byteCode.at(index) = (value % 256);
		byteCode.at(index + 1) = (value / 256);
	}
	void Compiler::AppendType(std::vector<Byte>& byteCode, TypePtr type)
	{
		switch (type->typeCode)
		{
		case TypeCode::Boolean:
		case TypeCode::Int32: {
			AppendTypeTag(byteCode, TypeTag::TYPE_I32);
			break;
		}
		case TypeCode::Int64: {
			AppendTypeTag(byteCode, TypeTag::TYPE_I64);
			break;
		}
		case TypeCode::Float32: {
			AppendTypeTag(byteCode, TypeTag::TYPE_F32);
			break;
		}
		case TypeCode::Float64: {
			AppendTypeTag(byteCode, TypeTag::TYPE_F64);
			break;
		}
		case TypeCode::String: {
			AppendTypeTag(byteCode, TypeTag::TYPE_STRING);
		}
		default:
			throw NotImplementedException();
		}
	}
	void Compiler::CompileBinary(std::shared_ptr<BinaryExpression> node,
		const std::unordered_map<ConstantKey, int>& constantMap,
		std::vector<Byte>& byteCode)
	{
		CompileExpression(node->left, constantMap, byteCode);
		CompileExpression(node->right, constantMap, byteCode);
		switch (node->nodeType)
		{
		case ExpressionType::Add: {
			AppendOp(byteCode, OpCode::ADD);
			break;
		}
		case ExpressionType::Subtract: {
			AppendOp(byteCode, OpCode::SUB);
			break;
		}
		case ExpressionType::Multiply: {
			AppendOp(byteCode, OpCode::MUL);
			break;
		}
		case ExpressionType::Divide: {
			AppendOp(byteCode, OpCode::DIV);
			break;
		}
		case ExpressionType::GreaterThan: {
			AppendOp(byteCode, OpCode::GT);
			break;
		}
		case ExpressionType::LessThan: {
			AppendOp(byteCode, OpCode::LT);
			break;
		}
		case ExpressionType::GreaterThanOrEqual: {
			AppendOp(byteCode, OpCode::GE);
			break;
		}
		case ExpressionType::LessThanOrEqual: {
			AppendOp(byteCode, OpCode::LE);
			break;
		}
		case ExpressionType::Equal: {
			AppendOp(byteCode, OpCode::EQ);
			break;
		}
		case ExpressionType::NotEqual: {
			AppendOp(byteCode, OpCode::NE);
			break;
		}
		default:
			throw NotImplementedException();
		}
		AppendType(byteCode, node->type);
	}
	void Compiler::CompileBlock(std::shared_ptr<BlockExpression> node,
		const std::unordered_map<ConstantKey, int>& constantMap,
		std::vector<Byte>& byteCode)
	{
		for (auto exp : node->expressions)
		{
			CompileExpression(exp, constantMap, byteCode);
		}
	}
	void Compiler::CompileExpression(
		ExpPtr node,
		const std::unordered_map<ConstantKey, int>& constantMap,
		std::vector<Byte>& byteCode)
	{
	}
	void Compiler::CompileConstant(
		std::shared_ptr<ConstantExpression> node,
		const std::unordered_map<ConstantKey, int>& constantMap,
		std::vector<Byte>& byteCode)
	{
		ConstantKey key{ node->type->typeCode, node->constant };
		if (constantMap.find(key) != constantMap.end())
		{
			int index = constantMap.at(key);
			AppendOp(byteCode, OpCode::PUSH_CONSTANT);
			AppendUShort(byteCode, index);
		}
		else
		{
			throw CompilerException(node->location,
				U"the given constant is not in the constant map");
		}
	}
	void Compiler::CompileParameter(std::shared_ptr<ParameterExpression> parameter,
		std::vector<Byte>& byteCode)
	{
		auto location = parameter->parameterLocation;
		switch (location.type)
		{
		case ParameterType::LocalVariable: {
			AppendOp(byteCode, OpCode::PUSH_STACK);
			break;
		}
		default:
			throw NotImplementedException();
		}
		AppendType(byteCode, parameter->type);
		AppendUShort(byteCode, location.offset);
	}
	void Compiler::CompileReturn(std::shared_ptr<ReturnExpression> node,
		const std::unordered_map<ConstantKey, int>& constantMap,
		std::vector<Byte>& byteCode)
	{
		CompileExpression(node->value, constantMap, byteCode);
		AppendOp(byteCode, OpCode::RETURN);
		AppendType(byteCode, node->type);
	}
	void Compiler::CompileConditional(
		std::shared_ptr<ConditionalExpression> node,
		const std::unordered_map<ConstantKey, int>& constantMap,
		std::vector<Byte>& byteCode)
	{
		CompileExpression(node->condition, constantMap, byteCode);
		AppendOp(byteCode, OpCode::JUMP_IF_FALSE);
		int index1 = static_cast<int>(byteCode.size());
		AppendUShort(byteCode, 0);

		CompileExpression(node->ifTrue, constantMap, byteCode);
		AppendOp(byteCode, OpCode::JUMP);
		int index2 = static_cast<int>(byteCode.size());
		AppendUShort(byteCode, 0);

		int target1 = static_cast<int>(byteCode.size());
		CompileExpression(node->ifFalse, constantMap, byteCode);
		int target2 = static_cast<int>(byteCode.size());

		WriteUShort(byteCode, index1, target1);
		WriteUShort(byteCode, index2, target2);
	}
	void Compiler::CompileDefault(std::shared_ptr<DefaultExpression> node,
		std::vector<Byte>& byteCode)
	{
		switch (node->type->typeCode)
		{
		case TypeCode::Void: {
			break;
		}
		default: {
			throw CompilerException(node->location,
				U"not supported type of default expression");
		}
		}
	}
	void Compiler::CompileInvocation(
		std::shared_ptr<InvocationExpression> node,
		const std::unordered_map<ConstantKey, int>& constantMap,
		std::vector<Byte>& byteCode)
	{
		for (auto arg : node->arguments)
		{
			CompileExpression(arg.value, constantMap, byteCode);
		}
		CompileExpression(node->expression, constantMap, byteCode);
		AppendOp(byteCode, OpCode::INVOKE);
	}
	void Compiler::CompileMemberAccess(
		std::shared_ptr<MemberAccessExpression> node,
		const std::unordered_map<ConstantKey, int>& constantMap,
		std::vector<Byte>& byteCode)
	{
		CompileExpression(node->object, constantMap, byteCode);
		AppendOp(byteCode, OpCode::PUSH_FIELD);
		AppendType(byteCode, node->type);
		// TO DO: field index
	}
} // namespace cygni