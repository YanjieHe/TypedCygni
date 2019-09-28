#include "DefinitionCollector.h"
#include "Exception.h"
#include <memory>
using std::make_shared;

DefinitionCollector::DefinitionCollector()
    : scope{make_shared<GlobalScope>()}, env{make_shared<GlobalTypeEnv>()}
{
}

void DefinitionCollector::Visit(UnaryExpression*)
{
}

void DefinitionCollector::Visit(BinaryExpression*)
{
}

void DefinitionCollector::Visit(ConstantExpression*)
{
}

void DefinitionCollector::Visit(BlockExpression* node)
{
    for (auto item : node->expressions)
    {
        item->Accept(this);
    }
}

void DefinitionCollector::Visit(ConditionalExpression*)
{
}

void DefinitionCollector::Visit(FullConditionalExpression*)
{
}

void DefinitionCollector::Visit(ParameterExpression*)
{
}

void DefinitionCollector::Visit(CallExpression*)
{
}

void DefinitionCollector::Visit(WhileExpression*)
{
}

void DefinitionCollector::Visit(VarExpression* node)
{
    // TO DO: handle the return value of the define function
    try
    {
        scope->Define(node->name);
        env->Define(node->name, node->type);
    }
    catch (NameDefinedException& ex)
    {
        throw SemanticException(node->position, ex.message);
    }
}

void DefinitionCollector::Visit(DefaultExpression*)
{
}

void DefinitionCollector::Visit(DefineExpression* node)
{
    fenv.Define(node->name, node->type);
}

void DefinitionCollector::Visit(NewExpression*)
{
}

void DefinitionCollector::Visit(AssignExpression*)
{
}

void DefinitionCollector::Visit(ReturnExpression*)
{
}

void DefinitionCollector::Visit(ImportExpression*)
{
}
