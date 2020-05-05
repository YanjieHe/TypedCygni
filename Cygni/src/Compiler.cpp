#include "Compiler.hpp"
#include "Exception.hpp"

namespace cygni
{
	ByteCode::ByteCode(std::u32string u32str)
	{
		std::string u8str = UTF32ToUTF8(u32str);
		for (Byte c : u8str)
		{
			Append(c);
		}
	}
	void ByteCode::Append(Byte byte)
	{
		bytes.push_back(byte);
	}
	void ByteCode::AppendOp(OpCode op)
	{
		bytes.push_back(static_cast<Byte>(op));
	}
	void ByteCode::AppendTypeTag(TypeTag tag)
	{
		bytes.push_back(static_cast<Byte>(tag));
	}
	void ByteCode::AppendUShort(int value)
	{
		bytes.push_back(value % 256);
		bytes.push_back(value / 256);
	}
	void ByteCode::WriteUShort(int index, int value)
	{
		bytes.at(index) = (value % 256);
		bytes.at(index + 1) = (value / 256);
	}
	void ByteCode::AppendType(TypePtr type)
	{
		switch (type->typeCode)
		{
		case TypeCode::Boolean:
		case TypeCode::Int32: {
			AppendTypeTag(TypeTag::TYPE_I32);
			break;
		}
		case TypeCode::Int64: {
			AppendTypeTag(TypeTag::TYPE_I64);
			break;
		}
		case TypeCode::Float32: {
			AppendTypeTag(TypeTag::TYPE_F32);
			break;
		}
		case TypeCode::Float64: {
			AppendTypeTag(TypeTag::TYPE_F64);
			break;
		}
		case TypeCode::String: {
			AppendTypeTag(TypeTag::TYPE_STRING);
		}
		default:
			throw NotImplementedException();
		}
	}
	int ByteCode::Size() const
	{
		return static_cast<int>(bytes.size());
	}
	void Compiler::CompileBinary(std::shared_ptr<BinaryExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		CompileExpression(node->left, constantMap, byteCode);
		CompileExpression(node->right, constantMap, byteCode);
		switch (node->nodeType)
		{
		case ExpressionType::Add: {
			byteCode.AppendOp(OpCode::ADD);
			break;
		}
		case ExpressionType::Subtract: {
			byteCode.AppendOp(OpCode::SUB);
			break;
		}
		case ExpressionType::Multiply: {
			byteCode.AppendOp(OpCode::MUL);
			break;
		}
		case ExpressionType::Divide: {
			byteCode.AppendOp(OpCode::DIV);
			break;
		}
		case ExpressionType::GreaterThan: {
			byteCode.AppendOp(OpCode::GT);
			break;
		}
		case ExpressionType::LessThan: {
			byteCode.AppendOp(OpCode::LT);
			break;
		}
		case ExpressionType::GreaterThanOrEqual: {
			byteCode.AppendOp(OpCode::GE);
			break;
		}
		case ExpressionType::LessThanOrEqual: {
			byteCode.AppendOp(OpCode::LE);
			break;
		}
		case ExpressionType::Equal: {
			byteCode.AppendOp(OpCode::EQ);
			break;
		}
		case ExpressionType::NotEqual: {
			byteCode.AppendOp(OpCode::NE);
			break;
		}
		default:
			throw NotImplementedException();
		}
		byteCode.AppendType(node->type);
	}
	void Compiler::CompileBlock(std::shared_ptr<BlockExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		for (auto exp : node->expressions)
		{
			CompileExpression(exp, constantMap, byteCode);
		}
	}
	void Compiler::CompileExpression(
		ExpPtr node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
	}
	void Compiler::CompileConstant(
		std::shared_ptr<ConstantExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		ConstantKey key{ node->type->typeCode, node->constant };
		if (constantMap.find(key) != constantMap.end())
		{
			int index = constantMap.at(key);
			byteCode.AppendOp(OpCode::PUSH_CONSTANT);
			byteCode.AppendUShort(index);
		}
		else
		{
			throw CompilerException(node->location,
				U"the given constant is not in the constant map");
		}
	}
	void Compiler::CompileParameter(std::shared_ptr<ParameterExpression> parameter,
		ByteCode& byteCode)
	{
		auto location = parameter->parameterLocation;
		switch (location.type)
		{
		case ParameterType::LocalVariable: {
			byteCode.AppendOp(OpCode::PUSH_STACK);
			break;
		}
		default:
			throw NotImplementedException();
		}
		byteCode.AppendType(parameter->type);
		byteCode.AppendUShort(location.offset);
	}
	void Compiler::CompileReturn(std::shared_ptr<ReturnExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		CompileExpression(node->value, constantMap, byteCode);
		byteCode.AppendOp(OpCode::RETURN);
		byteCode.AppendType(node->type);
	}
	void Compiler::CompileConditional(std::shared_ptr<ConditionalExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		CompileExpression(node->condition, constantMap, byteCode);
		byteCode.AppendOp(OpCode::JUMP_IF_FALSE);
		int index1 = byteCode.Size();
		byteCode.AppendUShort(0);

		CompileExpression(node->ifTrue, constantMap, byteCode);
		byteCode.AppendOp(OpCode::JUMP);
		int index2 = byteCode.Size();
		byteCode.AppendUShort(0);

		int target1 = byteCode.Size();
		CompileExpression(node->ifFalse, constantMap, byteCode);
		int target2 = byteCode.Size();

		byteCode.WriteUShort(index1, target1);
		byteCode.WriteUShort(index2, target2);
	}
	void Compiler::CompileDefault(std::shared_ptr<DefaultExpression> node,
		ByteCode& byteCode)
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
	void Compiler::CompileInvocation(std::shared_ptr<InvocationExpression> node,
		const ConstantMap& constantMap,
		ByteCode& byteCode)
	{
		for (auto arg : node->arguments)
		{
			CompileExpression(arg.value, constantMap, byteCode);
		}
		CompileExpression(node->expression, constantMap, byteCode);
		byteCode.AppendOp(OpCode::INVOKE);
	}
	void Compiler::CompileMemberAccess(std::shared_ptr<MemberAccessExpression> node,
		const ConstantMap& constantMap, ByteCode& byteCode)
	{
		CompileExpression(node->object, constantMap, byteCode);
		byteCode.AppendOp(OpCode::PUSH_FIELD);
		byteCode.AppendType(node->type);
		auto objType = node->object->type;
		if (objType->typeCode == TypeCode::Module)
		{
			auto modType = std::static_pointer_cast<ModuleType>(objType);
		}
		else
		{

		}
		// TO DO: field index
	}
} // namespace cygni