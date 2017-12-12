#ifndef COMPILER_H
#define COMPILER_H

#include "Visitor.h"
#include "Scope.h"
#include "DebugInfo.h"
#include "OpCode.h"
#include "Function.h"

#include <vector>
#include <cstdint>

using byte = unsigned char;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

class Compiler: public Visitor
{
public:
	vector<byte>* code;
	DebugInfo& debugInfo;
	LocationRecord& record;
	vector<Function*> functions;

    Compiler(DebugInfo& debugInfo, LocationRecord& record);
    Compiler(DebugInfo& debugInfo, LocationRecord& record, vector<byte>* code);

	void Visit(UnaryExpression* node) override;
	void Visit(BinaryExpression* node) override;
	void Visit(ConstantExpression* node) override;
	void Visit(BlockExpression* node) override;
	void Visit(ConditionalExpression* node) override;
	void Visit(FullConditionalExpression* node) override;
	void Visit(ParameterExpression* node) override;
	void Visit(CallExpression* node) override;
	void Visit(WhileExpression* node) override;
	void Visit(VarExpression* node) override;
	void Visit(DefaultExpression* node) override;
	void Visit(DefineExpression* node) override;
	void Visit(NewExpression* node) override;
	void Visit(AssignExpression* node) override;
	void Visit(ReturnExpression* node) override;

	void Emit(OpCode op);
	i32 CurrentIndex();

	void AppendBytes(byte* bytes, int length);
	void AppendUShort(unsigned short x);
	void AppendInt(i32 x);
	void AppendLong(i64 x);
	void AppendFloat(f32 x);
	void AppendDouble(f64 x);

	void WriteBytes(int offset, byte* bytes, int length);
	void WriteUShort(int offset, unsigned short x);
	void WriteInt(int offset, i32 x);
	void WriteLong(int offset, i64 x);
	void WriteFloat(int offset, f32 x);
	void WriteDouble(int offset, f64 x);
};

#endif // COMPILER_H 
