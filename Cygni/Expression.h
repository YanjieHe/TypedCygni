#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ExpressionKind.h"
#include "Type.h"
#include <memory>
#include <string>
#include <vector>
using std::shared_ptr;
using std::vector;
using std::wstring;

class Expression;
class UnaryExpression;
class BinaryExpression;
class ConstantExpression;
class BlockExpression;
class ConditionalExpression;
class FullConditionalExpression;
class ParameterExpression;
class CallExpression;
class WhileExpression;
class VarExpression;
class DefaultExpression;
class DefineExpression;
class NewExpression;
class AssignExpression;
class ReturnExpression;
class ImportExpression;

using ExpressionPtr = shared_ptr<Expression>;
using UnaryExpressionPtr = shared_ptr<UnaryExpression>;
using BinaryExpressionPtr = shared_ptr<BinaryExpression>;
using ConstantExpressionPtr = shared_ptr<ConstantExpression>;
using BlockExpressionPtr = shared_ptr<BlockExpression>;
using ConditionalExpressionPtr = shared_ptr<ConditionalExpression>;
using FullConditionalExpressionPtr = shared_ptr<FullConditionalExpression>;
using ParameterExpressionPtr = shared_ptr<ParameterExpression>;
using CallExpressionPtr = shared_ptr<CallExpression>;
using WhileExpressionPtr = shared_ptr<WhileExpression>;
using VarExpressionPtr = shared_ptr<VarExpression>;
using DefaultExpressionPtr = shared_ptr<DefaultExpression>;
using DefineExpressionPtr = shared_ptr<DefineExpression>;
using NewExpressionPtr = shared_ptr<NewExpression>;
using AssignExpressionPtr = shared_ptr<AssignExpression>;
using ReturnExpressionPtr = shared_ptr<ReturnExpression>;
using ImportExpressionPtr = shared_ptr<ImportExpression>;

class Visitor;

class Expression
{
public:
	ExpressionKind kind;
    int ID;
    virtual void Accept(Visitor* visitor) = 0;
    Type type;
    virtual ~Expression();

    static UnaryExpressionPtr Convert(ExpressionPtr node, Type type);

protected:
    Expression(ExpressionKind kind);
    Expression(ExpressionKind kind, Type type);
};

class UnaryExpression : public Expression
{
public:
    ExpressionPtr operand;
    UnaryExpression(ExpressionKind kind, ExpressionPtr operand);
	void Accept(Visitor* visitor) override;
};

class BinaryExpression : public Expression
{
public:
    ExpressionPtr left;
    ExpressionPtr right;
    BinaryExpression(ExpressionKind kind, ExpressionPtr left,
                     ExpressionPtr right);
	void Accept(Visitor* visitor) override;
};

class Constant
{
public:
	TypeTag tag;
	wstring text;
	Constant(TypeTag tag, wstring text);

	int GetInt();
	long GetLong();
	float GetFloat();
	double GetDouble();
};

class ConstantExpression : public Expression
{
public:
	Constant constant;
	ConstantExpression(Constant constant);
	void Accept(Visitor* visitor) override;
};

class BlockExpression : public Expression
{
public:
    vector<ExpressionPtr> expressions;
    BlockExpression(vector<ExpressionPtr> expressions);
	void Accept(Visitor* visitor) override;
};

class ConditionalExpression : public Expression
{
public:
    ExpressionPtr test;
    ExpressionPtr ifTrue;
    ConditionalExpression(ExpressionPtr test, ExpressionPtr ifTrue);
	void Accept(Visitor* visitor) override;
};

class FullConditionalExpression : public ConditionalExpression
{
public:
    ExpressionPtr ifFalse;
    FullConditionalExpression(ExpressionPtr test, ExpressionPtr ifTrue,
                              ExpressionPtr ifFalse);
	void Accept(Visitor* visitor) override;
};

class ParameterExpression : public Expression
{
public:
	wstring name;
	ParameterExpression(wstring name);
    ParameterExpression(wstring name, Type type);
	void Accept(Visitor* visitor) override;
};

class CallExpression : public Expression
{
public:
    ExpressionPtr procedure;
    vector<ExpressionPtr> arguments;
    CallExpression(ExpressionPtr procedure, vector<ExpressionPtr> arguments);
	void Accept(Visitor* visitor) override;
};

class WhileExpression : public Expression
{
public:
    ExpressionPtr condition;
    ExpressionPtr body;
    WhileExpression(ExpressionPtr condition, ExpressionPtr body);
	void Accept(Visitor* visitor) override;
};

class VarExpression : public Expression
{
public:
	wstring name;
    ExpressionPtr value;
    VarExpression(wstring name, ExpressionPtr value);
	void Accept(Visitor* visitor) override;
};

class DefaultExpression : public Expression
{
public:
    DefaultExpression(Type type);
	void Accept(Visitor* visitor) override;
};

class DefineExpression : public Expression
{
public:
	wstring name;
    vector<ParameterExpressionPtr> parameters;
    ExpressionPtr body;
	int frameSize;
    DefineExpression(wstring name, vector<ParameterExpressionPtr> parameters,
                     ExpressionPtr body, Type type);
	void Accept(Visitor* visitor) override;
};

class NewExpression : public Expression
{
public:
    vector<ExpressionPtr> initializers;
    NewExpression(vector<ExpressionPtr> initializers);
	void Accept(Visitor* visitor) override;
};

class AssignExpression : public Expression
{
public:
    ParameterExpressionPtr variable;
    ExpressionPtr value;
    AssignExpression(ParameterExpressionPtr variable, ExpressionPtr value);
	void Accept(Visitor* visitor) override;
};

class ReturnExpression : public Expression
{
public:
    ExpressionPtr value;
    ReturnExpression(ExpressionPtr value);
	void Accept(Visitor* visitor) override;
};

class ImportExpression : public Expression
{
public:
    wstring name;
    ImportExpression(wstring name);
    void Accept(Visitor* visitor) override;
};

class ModuleExpression : public BlockExpression
{
public:
    wstring name;
    ModuleExpression(wstring name, vector<ExpressionPtr> expressions);
};
#endif // EXPRESSION_H
