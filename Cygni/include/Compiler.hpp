#ifndef CYGNI_COMPILER_HPP
#define CYGNI_COMPILER_HPP
#include "Expression.hpp"
#include <cstdint>
#include <vector>
namespace cygni
{
	using Byte = uint8_t;

	class Compiler
	{
	public:
		std::vector<Byte> StringToBytes(std::u32string u32str);
		void Append(std::vector<Byte>& byteCode, Byte byte);
		void AppendOp(std::vector<Byte>& byteCode, OpCode op);
		void AppendTypeTag(std::vector<Byte>& byteCode, TypeTag tag);
		void AppendUShort(std::vector<Byte>& byteCode, int value);
		void WriteUShort(std::vector<Byte>& byteCode, int index, int value);
		void AppendType(std::vector<Byte>& byteCode, TypePtr type);
		//void CompileExpression(ExpPtr node, std::vector<Byte>& byteCode);
		void CompileBinary(std::shared_ptr<BinaryExpression> node,
			const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode);
		void CompileBlock(std::shared_ptr<BlockExpression> node,
			const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode);
		void CompileExpression(ExpPtr node, const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode);
		void CompileConstant(std::shared_ptr<ConstantExpression> node,
			const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode);
		//void CompileClassInfo(std::shared_ptr<ClassInfo> info);
		//void CompileModuleInfo(std::shared_ptr<ModuleInfo> info);
		//void CompileFieldDef(const FieldDef &field);
		//void CompileMethodDef(const MethodDef &method);
		void CompileParameter(std::shared_ptr<ParameterExpression> parameter, std::vector<Byte>& byteCode);
		void CompileReturn(std::shared_ptr<ReturnExpression> node,
			const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode);
		void CompileConditional(std::shared_ptr<ConditionalExpression> node,
			const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode);
		void CompileDefault(std::shared_ptr<DefaultExpression> node, std::vector<Byte>& byteCode);
		void CompileInvocation(std::shared_ptr<InvocationExpression> node,
			const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode);
		void CompileMemberAccess(std::shared_ptr<MemberAccessExpression> node,
			const std::unordered_map<ConstantKey, int>& constantMap, std::vector<Byte>& byteCode);
		//void CompileNewExpression(std::shared_ptr<NewExpression> node);
		//void CompileVarDefExpression(std::shared_ptr<VarDefExpression> node);
		//void CompileWhileLoop(std::shared_ptr<WhileExpression> node);
	};
} // namespace cygni

#endif // CYGNI_COMPILER_HPP