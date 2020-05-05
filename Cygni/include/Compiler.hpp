#ifndef CYGNI_COMPILER_HPP
#define CYGNI_COMPILER_HPP
#include "Expression.hpp"
#include <cstdint>
#include <vector>
namespace cygni
{
	using Byte = uint8_t;

	class ByteCode
	{
	public:
		std::vector<Byte> bytes;
		ByteCode() = default;
		explicit ByteCode(std::u32string u32str);
		void Append(Byte byte);
		void AppendOp(OpCode op);
		void AppendTypeTag(TypeTag tag);
		void AppendUShort(int value);
		void WriteUShort(int index, int value);
		void AppendType(TypePtr type);
		int Size() const;
	};

	using ConstantMap = std::unordered_map<ConstantKey, int>;

	class Compiler
	{
	public:
		//void CompileExpression(ExpPtr node, ByteCode& byteCode);
		void CompileBinary(std::shared_ptr<BinaryExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileBlock(std::shared_ptr<BlockExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileExpression(ExpPtr node, const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileConstant(std::shared_ptr<ConstantExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		//void CompileClassInfo(std::shared_ptr<ClassInfo> info);
		//void CompileModuleInfo(std::shared_ptr<ModuleInfo> info);
		//void CompileFieldDef(const FieldDef &field);
		//void CompileMethodDef(const MethodDef &method);
		void CompileParameter(std::shared_ptr<ParameterExpression> parameter, ByteCode& byteCode);
		void CompileReturn(std::shared_ptr<ReturnExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileConditional(std::shared_ptr<ConditionalExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileDefault(std::shared_ptr<DefaultExpression> node, ByteCode& byteCode);
		void CompileInvocation(std::shared_ptr<InvocationExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileMemberAccess(std::shared_ptr<MemberAccessExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		//void CompileNewExpression(std::shared_ptr<NewExpression> node);
		//void CompileVarDefExpression(std::shared_ptr<VarDefExpression> node);
		//void CompileWhileLoop(std::shared_ptr<WhileExpression> node);
	};
} // namespace cygni

#endif // CYGNI_COMPILER_HPP