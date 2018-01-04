#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ExpressionKind.h"
#include "Type.h"
#include <string>
#include <vector>
using std::vector;
using std::wstring;

class Expression;
class UnaryExpression;
class BinaryExpression;
class Visitor;

class Expression
{
public:
	ExpressionKind kind;
    int ID;
    virtual void Accept(Visitor* visitor) = 0;
    Type type;
    virtual ~Expression();

    static UnaryExpression* Convert(Expression* node, Type type);

protected:
    Expression(ExpressionKind kind);
    Expression(ExpressionKind kind, Type type);
};

class UnaryExpression : public Expression
{
public:
	Expression* operand;
	UnaryExpression(ExpressionKind kind, Expression* operand);
	void Accept(Visitor* visitor) override;
};

class BinaryExpression : public Expression
{
public:
	Expression* left;
	Expression* right;
	BinaryExpression(ExpressionKind kind, Expression* left, Expression* right);
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
	vector<Expression*> expressions;
	BlockExpression(vector<Expression*> expressions);
	void Accept(Visitor* visitor) override;
};

class ConditionalExpression : public Expression
{
public:
	Expression* test;
	Expression* ifTrue;
	ConditionalExpression(Expression* test, Expression* ifTrue);
	void Accept(Visitor* visitor) override;
};

class FullConditionalExpression : public ConditionalExpression
{
public:
	Expression* ifFalse;
    FullConditionalExpression(Expression* test, Expression* ifTrue,
                              Expression* ifFalse);
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
	Expression* procedure;
	vector<Expression*> arguments;
	CallExpression(Expression* procedure, vector<Expression*> arguments);
	void Accept(Visitor* visitor) override;
};

class WhileExpression : public Expression
{
public:
	Expression* condition;
	Expression* body;
	WhileExpression(Expression* condition, Expression* body);
	void Accept(Visitor* visitor) override;
};

class VarExpression : public Expression
{
public:
	wstring name;
	Expression* value;
	VarExpression(wstring name, Expression* value);
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
	vector<ParameterExpression*> parameters;
	Expression* body;
	int frameSize;
    DefineExpression(wstring name, vector<ParameterExpression*> parameters,
                     Expression* body, Type type);
	void Accept(Visitor* visitor) override;
};

class NewExpression : public Expression
{
public:
	vector<Expression*> initializers;
	NewExpression(vector<Expression*> initializers);
	void Accept(Visitor* visitor) override;
};

class AssignExpression : public Expression
{
public:
	ParameterExpression* variable;
	Expression* value;
	AssignExpression(ParameterExpression* variable, Expression* value);
	void Accept(Visitor* visitor) override;
};

class ReturnExpression : public Expression
{
public:
	Expression* value;
	ReturnExpression(Expression* value);
	void Accept(Visitor* visitor) override;
};
#endif // EXPRESSION_H
