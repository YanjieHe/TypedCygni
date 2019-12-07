#include "Expression.h"
#include "Exception.h"
#include "Visitor.h"

using std::make_shared;

Expression::Expression(ExpressionKind kind) : kind{kind}
{
    this->ID = -1;
}

Expression::Expression(ExpressionKind kind, Type type) : kind{kind}, type{type}
{
    this->ID = -1;
}

Expression::~Expression()
{
}

UnaryExpressionPtr Expression::Convert(ExpressionPtr node, Type type)
{
    UnaryExpressionPtr expression =
        make_shared<UnaryExpression>(ExpressionKind::Convert, node);
    expression->type = type;
    return expression;
}

UnaryExpression::UnaryExpression(ExpressionKind kind, ExpressionPtr operand)
    : Expression(kind), operand{operand}
{
}

void UnaryExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

BinaryExpression::BinaryExpression(ExpressionKind kind, ExpressionPtr left,
                                   ExpressionPtr right)
    : Expression(kind), left{left}, right{right}
{
}

void BinaryExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

Constant::Constant(TypeTag tag, wstring text) : tag{tag}, text{text}
{
}

int Constant::GetInt()
{
	return std::stoi(text);
}

long Constant::GetLong()
{
	return std::stol(text);
}

float Constant::GetFloat()
{
	return std::stof(text);
}

double Constant::GetDouble()
{
	return std::stod(text);
}

ConstantExpression::ConstantExpression(Constant constant)
    : Expression(ExpressionKind::Constant), constant{constant}
{
}

void ConstantExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

BlockExpression::BlockExpression(vector<ExpressionPtr> expressions)
    : Expression(ExpressionKind::Block), expressions{expressions}
{
}

void BlockExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

ConditionalExpression::ConditionalExpression(ExpressionPtr test,
                                             ExpressionPtr ifTrue)
    : Expression(ExpressionKind::Conditional), test{test}, ifTrue{ifTrue}
{
}

void ConditionalExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

FullConditionalExpression::FullConditionalExpression(ExpressionPtr test,
                                                     ExpressionPtr ifTrue,
                                                     ExpressionPtr ifFalse)
    : ConditionalExpression(test, ifTrue), ifFalse{ifFalse}
{
}

void FullConditionalExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

ParameterExpression::ParameterExpression(wstring name)
    : Expression(ExpressionKind::Parameter), name{name}
{
}

ParameterExpression::ParameterExpression(wstring name, Type type)
    : Expression(ExpressionKind::Parameter, type), name{name}
{
}

void ParameterExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

CallExpression::CallExpression(ExpressionPtr procedure,
                               vector<ExpressionPtr> arguments)
    : Expression(ExpressionKind::Call), procedure{procedure}, arguments{
                                                                  arguments}
{
}

void CallExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

WhileExpression::WhileExpression(ExpressionPtr condition, ExpressionPtr body)
    : Expression(ExpressionKind::While), condition{condition}, body{body}
{
}

void WhileExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

VarExpression::VarExpression(wstring name, ExpressionPtr value)
    : Expression(ExpressionKind::Var), name{name}, value{value}
{
}

void VarExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

DefaultExpression::DefaultExpression(Type type)
    : Expression(ExpressionKind::Default, type)
{
}

void DefaultExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

DefineExpression::DefineExpression(wstring name,
                                   vector<ParameterExpressionPtr> parameters,
                                   ExpressionPtr body, Type type)
    : Expression(ExpressionKind::Define, type), name{name},
      parameters{parameters}, body{body}
{
}

void DefineExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

AssignExpression::AssignExpression(ParameterExpressionPtr variable,
                                   ExpressionPtr value)
    : Expression(ExpressionKind::Assign), variable{variable}, value{value}
{
}

void AssignExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

ReturnExpression::ReturnExpression(ExpressionPtr value)
    : Expression(ExpressionKind::Return), value{value}
{
}

void ReturnExpression::Accept(Visitor* visitor)
{
	visitor->Visit(this);
}

ImportExpression::ImportExpression(std::wstring name)
    : Expression(ExpressionKind::Import), name{name}
{
}

void ImportExpression::Accept(Visitor* visitor)
{
    visitor->Visit(this);
}

ModuleExpression::ModuleExpression(std::wstring name,
                                   vector<ExpressionPtr> expressions)
    : BlockExpression(expressions), name{name}
{
}
