#include "TypeChecker.h"
#include "Exception.h"
#include <iostream>

using namespace std;

TypeChecker::TypeChecker(DebugInfo& debugInfo, LocationRecord& record)
	:debugInfo{debugInfo}, record{record}
{
	this->scope = new GlobalScope();
	this->fenv = new FunctionEnv();
	this->env = new TypeEnv(fenv);
}

void TypeChecker::Visit(UnaryExpression* node)
{
	node->operand->Accept(this);
	if (node->operand->GetType()->IsInt())
	{
		if (node->kind == ExpressionKind::UnaryPlus)
		{
			node->SetType(Type::Int());
		}
		else if (node->kind == ExpressionKind::Negate)
		{
			node->SetType(Type::Int());
		}
		else
		{
			throw SemanticException(debugInfo.Locate(node),
					L"not supported unary operation");
		}
	}
	else if (node->operand->GetType()->IsDouble())
	{
		if (node->kind == ExpressionKind::UnaryPlus)
		{
			node->SetType(Type::Double());
		}
		else if (node->kind == ExpressionKind::Negate)
		{
			node->SetType(Type::Double());
		}
		else
		{
			throw SemanticException(debugInfo.Locate(node),
					L"not supported unary operation");
		}
	}
	else if (node->operand->GetType()->IsBoolean())
	{
		if (node->kind == ExpressionKind::Not)
		{
			node->SetType(Type::Boolean());
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

	if (node->left->GetType()->IsInt() && node->right->GetType()->IsInt())
	{
		switch (node->kind)
		{
			case ExpressionKind::Add:
			case ExpressionKind::Subtract:
			case ExpressionKind::Multiply:
			case ExpressionKind::Divide:
			case ExpressionKind::Modulo:
				node->SetType(Type::Int());
				return;
			case ExpressionKind::GreaterThan:
			case ExpressionKind::LessThan:
			case ExpressionKind::GreaterThanOrEqual:
			case ExpressionKind::LessThanOrEqual:
			case ExpressionKind::Equal:
			case ExpressionKind::NotEqual:
				node->SetType(Type::Boolean());
				return;
			case ExpressionKind::Assign:
				node->SetType(Type::Int());
				return;
			default:
				throw SemanticException(debugInfo.Locate(node),
						L"not supported binary operation");
		}
	}
	else if (node->left->GetType()->IsDouble() && node->right->GetType()->IsDouble())
	{
		switch (node->kind)
		{
			case ExpressionKind::Add:
			case ExpressionKind::Subtract:
			case ExpressionKind::Multiply:
			case ExpressionKind::Divide:
			case ExpressionKind::Modulo:
				node->SetType(Type::Double());
				return;
			case ExpressionKind::GreaterThan:
			case ExpressionKind::LessThan:
			case ExpressionKind::GreaterThanOrEqual:
			case ExpressionKind::LessThanOrEqual:
			case ExpressionKind::Equal:
			case ExpressionKind::NotEqual:
				node->SetType(Type::Boolean());
				return;
			case ExpressionKind::Assign:
				node->SetType(Type::Double());
				return;
			default:
				throw SemanticException(debugInfo.Locate(node),
						L"not supported binary operation");
		}
	}
	else
	{
		throw SemanticException(debugInfo.Locate(node),
				L"not supported binary operation");
	}
}

void TypeChecker::Visit(ConstantExpression* node)
{
	if (node->constant.tag == TypeTag::Int)
	{
		node->SetType(Type::Int());
	}
	else if (node->constant.tag == TypeTag::Double)
	{
		node->SetType(Type::Double());
	}
	else if (node->constant.tag == TypeTag::Boolean)
	{
		node->SetType(Type::Boolean());
	}
	else if (node->constant.tag == TypeTag::String)
	{
		node->SetType(Type::String());
	}
	else
	{
		throw SemanticException(debugInfo.Locate(node),
				L"unsupported constant type");
	}
}

void TypeChecker::Visit(BlockExpression* node)
{
	for (Expression* expression: node->expressions)
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
		throw SemanticException(debugInfo.Locate(node),
				L"not defined variable");
	}
	else
	{
		record.Record(node, location);
		node->SetType(env->Find(node->name));
	}
}

void TypeChecker::Visit(CallExpression* node)
{
	node->procedure->Accept(this);
	vector<Type*> argsType;
	for (Expression* item: node->arguments)
	{
		item->Accept(this);
		argsType.push_back(item->GetType()->Clone());
	}

	if (node->GetType()->tag == TypeTag::Function)
	{
		FunctionType* ft = (FunctionType*) node->procedure->GetType();
		if (ft->ParametersMatch(argsType))
		{
			node->SetType(ft->returnType->Clone());
			return;
		}
		else
		{
			throw SemanticException(debugInfo.Locate(node), L"arguments type error");
		}
	}
	else
	{
		throw SemanticException(debugInfo.Locate(node), L"procedure type error");
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
	if (node->GetType()->tag == TypeTag::Unknown)
	{
		node->SetType(node->value->GetType());
		scope->Define(node->name);
		env->Define(node->name, node->GetType());
	}
	else
	{
		throw SemanticException(debugInfo.Locate(node), L"not supported yet");
	}
}

void TypeChecker::Visit(DefaultExpression* node)
{
	// TO DO
	return;
}

void TypeChecker::Visit(DefineExpression* node)
{
	Scope* prev = scope;
	scope = new FunctionScope(scope);
	env = new TypeEnv(fenv, env);
	for (ParameterExpression* item: node->parameters)
	{
		scope->Define(item->name);
		env->Define(item->name, item->GetType());
	}
	node->body->Accept(this);
	fenv->Define(node->name, node->GetType());
	scope = prev;
	env = env->parent;
}

void TypeChecker::Visit(NewExpression* node)
{
	// TO DO
}

