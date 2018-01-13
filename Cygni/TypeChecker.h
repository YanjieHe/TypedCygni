#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "DebugInfo.h"
#include "Scope.h"
#include "TypeEnv.h"
#include "Visitor.h"

class TypeChecker : public Visitor
{
public:
	DebugInfo& debugInfo;
	LocationRecord& record;
    ScopePtr scope;
    FunctionList& fenv;
    TypeEnvPtr env;
    TypeChecker(DebugInfo& debugInfo, LocationRecord& record,
                GlobalScopePtr globalScope, FunctionList& fenv);

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

    void VisitBinary(BinaryExpression* node, Type typeOfArithmetic);
};

#endif // TYPECHECKER_H
