#include "Expression.h"
#include "Visitor.h"

Expression::Expression(ExpressionKind kind)
	:kind{kind}
{
	this->type = Type::Unknown();
	this->ID = -1;
}

UnaryExpression::UnaryExpression(ExpressionKind kind, Expression* operand)
	:Expression(kind), operand{operand}
{
}

void UnaryExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

BinaryExpression::BinaryExpression(ExpressionKind kind,
		Expression* left, Expression* right)
	:Expression(kind), left{left}, right{right}
{
}

void BinaryExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

Constant::Constant(TypeTag tag, wstring text)
	:tag{tag}, text{text}
{
}

ConstantExpression::ConstantExpression(Constant constant)
	:Expression(ExpressionKind::Constant), constant{constant}
{
}

void ConstantExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

BlockExpression::BlockExpression(vector<Expression*> expressions)
	:Expression(ExpressionKind::Block), expressions{expressions}
{
}

void BlockExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

ConditionalExpression::ConditionalExpression(Expression* test,
		Expression* ifTrue)
	:Expression(ExpressionKind::Conditional), test{test}, ifTrue{ifTrue}
{
}

void ConditionalExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

FullConditionalExpression::FullConditionalExpression(Expression* test,
		Expression* ifTrue, Expression* ifFalse)
	:ConditionalExpression(test, ifTrue), ifFalse{ifFalse}
{
}

void FullConditionalExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

ParameterExpression::ParameterExpression(wstring name)
	:Expression(ExpressionKind::Parameter), name{name}
{
}

void ParameterExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

CallExpression::CallExpression(Expression* procedure, vector<Expression*> arguments)
	:Expression(ExpressionKind::Call), procedure{procedure}, arguments{arguments}
{
}

void CallExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

WhileExpression::WhileExpression(Expression* condition, Expression* body)
	:Expression(ExpressionKind::While), condition{condition}, body{body}
{
}

void WhileExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

VarExpression::VarExpression(wstring name, Expression* value)
	:Expression(ExpressionKind::Var), name{name}, value{value}
{
}

void VarExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

DefaultExpression::DefaultExpression(Type* type)
	:Expression(ExpressionKind::Default)
{
	this->type = type;
}

void DefaultExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

DefineExpresion::DefineExpresion(wstring name, vector<ParameterExpression*> parameters,
		Expression* body)
	:Expression(ExpressionKind::Define), name{name}, parameters{parameters}, body{body}
{
}

void DefineExpresion::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}
