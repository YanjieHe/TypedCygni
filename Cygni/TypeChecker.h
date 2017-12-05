#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "Visitor.h"
#include "Scope.h"
#include "TypeEnv.h"
#include "DebugInfo.h"

class TypeChecker: public Visitor
{
public:
	DebugInfo& debugInfo;
	LocationRecord& record;
	Scope* scope;
	FunctionEnv* fenv;
	TypeEnv* env;
    TypeChecker(DebugInfo& debugInfo, LocationRecord& record);

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
};

#endif // TYPECHECKER_H 
