#ifndef BYTECODEGENERATOR_H
#define BYTECODEGENERATOR_H

#include "DebugInfo.h"
#include "Function.h"
#include "OpCode.h"
#include "PrimitiveType.h"
#include "Scope.h"
#include "Visitor.h"
#include <vector>

using ByteCode = vector<byte>;

class ByteCodeGenerator : public Visitor
{
public:
    ByteCode* code;
	DebugInfo& debugInfo;
	LocationRecord& record;
    vector<Function> functions;
    ByteCode globalCode;

    ByteCodeGenerator(DebugInfo& debugInfo, LocationRecord& record);

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
    void Visit(ImportExpression* node) override;

	void Emit(OpCode op);
	i32 CurrentIndex();

    void AppendBytes(byte* bytes, int length);
    void AppendBytes(ByteCode& code, byte* bytes, int length);

    void AppendUShort(ByteCode& code, u16 x);
    void AppendUShort(u16 x);

	void AppendInt(i32 x);
	void AppendLong(i64 x);
	void AppendFloat(f32 x);
	void AppendDouble(f64 x);

    void AppendInt(ByteCode& code, i32 x);
    void AppendWChar(ByteCode& code, wchar_t c);
    void AppendWString(ByteCode& code, std::wstring& str);

	void WriteBytes(int offset, byte* bytes, int length);
    void WriteUShort(int offset, u16 x);
	void WriteInt(int offset, i32 x);
	void WriteLong(int offset, i64 x);
	void WriteFloat(int offset, f32 x);
	void WriteDouble(int offset, f64 x);

    ByteCode GetTopCode();
    ByteCode GetFunctionCode(Function& f);
};

#endif // BYTECODEGENERATOR_H
