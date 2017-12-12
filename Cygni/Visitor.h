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
};

class TreeViewer: public Visitor
{
public:
	TreeViewer();

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

private:
	int depth;

	void Indent();
};

#endif // VISITOR_H 
