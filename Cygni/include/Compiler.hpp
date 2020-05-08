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
		void AppendUInt(uint32_t value);
		void AppendType(TypePtr type);
		void AppendString(const std::u32string& u32str);
		void AppendByteCode(const ByteCode& other);
		int Size() const;
	};

	using ConstantMap = std::unordered_map<ConstantKey, int>;

	class Compiler
	{
	public:
		Project& project;
		explicit Compiler(Project& project);
		ByteCode Compile();
		void CompileBinary(std::shared_ptr<BinaryExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileBlock(std::shared_ptr<BlockExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileExpression(ExpPtr node, const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileConstant(std::shared_ptr<ConstantExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileClassInfo(std::shared_ptr<ClassInfo> info, ByteCode& byteCode);
		void CompileModuleInfo(std::shared_ptr<ModuleInfo> info, ByteCode& byteCode);
		//void CompileFieldDef(const FieldDef &field, ByteCode& byteCode);
		ByteCode CompileMethodDef(const MethodDef &method);
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
		void CompileNewExpression(std::shared_ptr<NewExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileVarDefExpression(std::shared_ptr<VarDefExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		void CompileAssign(std::shared_ptr<BinaryExpression> node,
			const ConstantMap& constantMap, ByteCode& byteCode);
		//void CompileWhileLoop(std::shared_ptr<WhileExpression> node);
	};
} // namespace cygni

#endif // CYGNI_COMPILER_HPP