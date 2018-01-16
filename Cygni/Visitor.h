#ifndef VISITOR_H
#define VISITOR_H

#include "Expression.h"

class Visitor
{
public:
	virtual void Visit(UnaryExpression* node) = 0;
	virtual void Visit(BinaryExpression* node) = 0;
	virtual void Visit(ConstantExpression* node) = 0;
	virtual void Visit(BlockExpression* node) = 0;
	virtual void Visit(ConditionalExpression* node) = 0;
	virtual void Visit(FullConditionalExpression* node) = 0;
	virtual void Visit(ParameterExpression* node) = 0;
	virtual void Visit(CallExpression* node) = 0;
	virtual void Visit(WhileExpression* node) = 0;
	virtual void Visit(VarExpression* node) = 0;
	virtual void Visit(DefaultExpression* node) = 0;
	virtual void Visit(DefineExpression* node) = 0;
	virtual void Visit(NewExpression* node) = 0;
	virtual void Visit(AssignExpression* node) = 0;
	virtual void Visit(ReturnExpression* node) = 0;
    virtual void Visit(ImportExpression* node) = 0;

    virtual ~Visitor()
    {
    }
};

#endif // VISITOR_H
