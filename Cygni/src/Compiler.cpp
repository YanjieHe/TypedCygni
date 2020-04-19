#include "Compiler.hpp"
#include "Exception.hpp"

namespace cygni {
	std::vector<Byte> Compiler::StringToBytes(std::u32string u32str)
	{
		std::vector<Byte> bytes;
		std::string u8str = UTF32ToUTF8(s);
		for (Byte c : u8str) {
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
		byteCode.push_back(static_cast<TypeTag>(tag));
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
		switch (type->typeCode) {
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
	void Compiler::CompileBinary(std::shared_ptr<BinaryExpression> node, const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode)
	{
		CompileExpression(node->left, constantMap, byteCode);
		CompileExpression(node->right, constantMap, byteCode);
		switch (node->nodeType) {
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
		case ExpressionType::DIV: {
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
	void Compiler::CompileBlock(std::shared_ptr<BlockExpression> node, const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode)
	{
		for (auto exp : node->expressions) {
			CompileExpression(exp, constantMap, byteCode);
		}
	}
	void Compiler::CompileExpression(ExpPtr node, const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode)
	{
	}
	void Compiler::CompileConstant(std::shared_ptr<ConstantExpression> node,
		const std::unordered_map<ConstantKey, int> &constantMap, std::vector<Byte>& byteCode)
	{
		ConstantKey key{ node->type->typeCode, node->constant };
		if (constantMap.find(key) != constantMap.end()) {
			int index = constantMap[key];
			Append(byteCode, OpCode::PUSH_CONSTANT);
			AppendUShort(byteCode, index);
		}
		else {
			throw CompilerException(node->location, U"the given constant is not in the constant map");
		}
	}
	void Compiler::CompileParameter(std::shared_ptr<ParameterExpression> parameter, std::vector<Byte>& byteCode)
	{
		auto location = parameter->parameterLocation;
		swtich(location.type) {
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
	void Compiler::CompileReturn(std::shared_ptr<ReturnExpression> node, const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode)
	{
		CompileExpression(node->value, constantMap, byteCode);
		AppendOp(byteCode, OpCode::RETURN);
		AppendType(byteCode, node->type);
	}
} // namespace cygni