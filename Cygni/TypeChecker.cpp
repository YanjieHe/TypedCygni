#include "TypeChecker.h"
#include "Exception.h"
#include <iostream>
#include <memory>
using namespace std;

TypeChecker::TypeChecker(DebugInfo& debugInfo, LocationRecord& record,
                         GlobalScopePtr globalScope, FunctionList& fenv)
    : debugInfo{debugInfo}, record{record}, scope{static_pointer_cast<Scope>(
                                                globalScope)},
      fenv{fenv}, env{make_shared<GlobalTypeEnv>()}
{
}

void TypeChecker::Visit(UnaryExpression* node)
{
	node->operand->Accept(this);
    if (node->operand->type.IsInt())
	{
		if (node->kind == ExpressionKind::UnaryPlus)
		{
            node->type = Type::Int();
		}
		else if (node->kind == ExpressionKind::Negate)
        {
            node->type = Type::Int();
		}
		else
		{
			throw SemanticException(debugInfo.Locate(node),
                                    L"not supported unary operation");
		}
	}
    else if (node->operand->type.IsDouble())
	{
		if (node->kind == ExpressionKind::UnaryPlus)
		{
            node->type = Type::Double();
		}
		else if (node->kind == ExpressionKind::Negate)
		{
            node->type = Type::Double();
		}
		else
		{
			throw SemanticException(debugInfo.Locate(node),
                                    L"not supported unary operation");
		}
	}
    else if (node->operand->type.IsBoolean())
	{
		if (node->kind == ExpressionKind::Not)
		{
            node->type = Type::Boolean();
		}
		else
		{
			throw SemanticException(debugInfo.Locate(node),
                                    L"not supported unary operation");
		}
	}
}

void TypeChecker::Visit(BinaryExpression* node)
{
	node->left->Accept(this);
    node->right->Accept(this);

    if (node->left->type.IsInt() && node->right->type.IsInt())
	{
        VisitBinary(node, Type::Int());
	}
    else if (node->left->type.IsDouble() && node->right->type.IsDouble())
    {
        VisitBinary(node, Type::Double());
    }
    else if (node->left->type.IsInt() && node->right->type.IsDouble())
    {
        Position p = debugInfo.Locate(node->left);
        node->left = Expression::Convert(node->left, Type::Double());
        debugInfo.Record(p, node->left);
        VisitBinary(node, Type::Double());
    }
    else if (node->left->type.IsDouble() && node->right->type.IsInt())
    {
        Position p = debugInfo.Locate(node->right);
        node->right = Expression::Convert(node->right, Type::Double());
        debugInfo.Record(p, node->right);
        VisitBinary(node, Type::Double());
	}
	else
	{
		throw SemanticException(debugInfo.Locate(node),
                                L"not supported binary operation");
	}
}

void TypeChecker::Visit(ConstantExpression* node)
{
    switch (node->constant.tag)
    {
    case TypeTag::Int:
        node->type = Type::Int();
        break;
    case TypeTag::Boolean:
        node->type = Type::Boolean();
        break;
    case TypeTag::Double:
        node->type = Type::Double();
        break;
    case TypeTag::String:
        node->type = Type::String();
        break;
    default:
        throw SemanticException(debugInfo.Locate(node),
                                L"unsupported constant type");
    }
}

void TypeChecker::Visit(BlockExpression* node)
{
    for (ExpressionPtr expression : node->expressions)
    {
        expression->Accept(this);
	}
}

void TypeChecker::Visit(ConditionalExpression* node)
{
	node->test->Accept(this);
	node->ifTrue->Accept(this);
}

void TypeChecker::Visit(FullConditionalExpression* node)
{
	node->test->Accept(this);
	node->ifTrue->Accept(this);
	node->ifFalse->Accept(this);
}

void TypeChecker::Visit(ParameterExpression* node)
{
	Location location = scope->Find(node->name);
	if (location.kind == LocationKind::Unknown)
	{
        int index = fenv.Find(node->name);
		if (index != -1)
		{
            record.Record(node, Location(LocationKind::FunctionID, index));
            node->type = fenv.ResolveType(node->name);
		}
		else
		{
			throw SemanticException(debugInfo.Locate(node),
                                    L"not defined variable: " + node->name);
		}
	}
	else
	{
		record.Record(node, location);
        node->type = env->Find(node->name);
	}
}

void TypeChecker::Visit(CallExpression* node)
{
	node->procedure->Accept(this);
    vector<Type> argsType;
    for (ExpressionPtr item : node->arguments)
	{
		item->Accept(this);
        argsType.push_back(item->type);
	}

    if (node->procedure->type.IsFunction())
	{
        Type ft = node->procedure->type;
        if (ft.ParametersMatch(argsType))
		{
            node->type = ft.ReturnType();
		}
		else
		{
            throw SemanticException(debugInfo.Locate(node),
                                    L"arguments type error");
		}
	}
	else
	{
        throw SemanticException(debugInfo.Locate(node),
                                L"procedure type error");
	}
}

void TypeChecker::Visit(WhileExpression* node)
{
	node->condition->Accept(this);
	node->body->Accept(this);
}

void TypeChecker::Visit(VarExpression* node)
{
	node->value->Accept(this);
    if (node->type.tag == TypeTag::Unknown)
	{
        node->type = node->value->type;
		if (scope->Define(node->name))
		{
			record.Record(node, scope->Find(node->name));
            env->Define(node->name, node->type);
		}
		else
		{
            throw SemanticException(debugInfo.Locate(node),
                                    L"name already defined");
		}
	}
	else
	{
		throw SemanticException(debugInfo.Locate(node), L"not supported yet");
	}
}

void TypeChecker::Visit(DefaultExpression*)
{
	// TO DO
    throw NotImplementedException();
}

void TypeChecker::Visit(DefineExpression* node)
{
    ScopePtr prev = scope;
    scope = make_shared<FunctionScope>(scope);
    TypeEnvPtr prev_env = env;
    env = make_shared<FunctionTypeEnv>(node->type, env);
    for (ParameterExpressionPtr item : node->parameters)
	{
		scope->Define(item->name);
        env->Define(item->name, item->type);
    }
	node->body->Accept(this);
    node->frameSize = scope->Size();
	scope = prev;
	env = prev_env;
}

void TypeChecker::Visit(NewExpression*)
{
    // TO DO
    throw NotImplementedException();
}

void TypeChecker::Visit(AssignExpression* node)
{
	node->variable->Accept(this);
	node->value->Accept(this);
    node->type = (node->variable->type);
}

void TypeChecker::Visit(ReturnExpression* node)
{
    if (!env->IsGlobal())
    {
        node->value->Accept(this);
        FunctionTypeEnvPtr ftenv = static_pointer_cast<FunctionTypeEnv>(env);
        if (node->value->type.Mathces(ftenv->type.ReturnType()))
        {
            node->type = (node->value->type);
        }
        else if (node->value->type.IsSubtypeOf(ftenv->type.ReturnType()))
        {
            throw NotImplementedException();
        }
        else
        {
            throw SemanticException(debugInfo.Locate(node),
                                    L"type of the return statement does not "
                                    L"match the return type of the function");
        }
    }
    else
    {
        throw SemanticException(debugInfo.Locate(node),
                                L"return statement should be in a function");
    }
}

void TypeChecker::Visit(ImportExpression*)
{
    // TO DO
    return;
}

void TypeChecker::VisitBinary(BinaryExpression* node, Type typeOfArithmetic)
{
    switch (node->kind)
    {
    case ExpressionKind::Add:
    case ExpressionKind::Subtract:
    case ExpressionKind::Multiply:
    case ExpressionKind::Divide:
    case ExpressionKind::Modulo:
        node->type = typeOfArithmetic;
        return;
    case ExpressionKind::GreaterThan:
    case ExpressionKind::LessThan:
    case ExpressionKind::GreaterThanOrEqual:
    case ExpressionKind::LessThanOrEqual:
    case ExpressionKind::Equal:
    case ExpressionKind::NotEqual:
        node->type = Type::Boolean();
        return;
    default:
        throw SemanticException(debugInfo.Locate(node),
                                L"not supported binary operation");
    }
}
