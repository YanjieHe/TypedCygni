#ifndef DEFINITIONCOLLECTOR_H
#define DEFINITIONCOLLECTOR_H

#include "Scope.h"
#include "Type.h"
#include "TypeEnv.h"
#include "Visitor.h"
#include <string>
using std::wstring;
class DefinitionCollector : public Visitor
{
public:
    DefinitionCollector();
    GlobalScopePtr scope;
    GlobalTypeEnvPtr env;
    FunctionList fenv;

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
};

#endif // DEFINITIONCOLLECTOR_H
