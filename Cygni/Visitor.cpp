#include "Visitor.h"
#include "ExpressionKind.h"
#include <iostream>

using namespace std;

void TreeViewer::Visit(UnaryExpression* node)
{
	Indent();
	wcout << expression_kind_to_wstring(node->kind) << endl;
	depth++;
	node->operand->Accept(this);
	depth--;
}

void TreeViewer::Visit(BinaryExpression* node)
{
	Indent();
	wcout << expression_kind_to_wstring(node->kind) << endl;
	depth++;
	node->left->Accept(this);
	node->right->Accept(this);
	depth--;
}

void TreeViewer::Visit(ConstantExpression* node)
{
	Indent();
	wcout << node->constant.text << endl;
}

void TreeViewer::Visit(BlockExpression* node)
{
	Indent();
	wcout << expression_kind_to_wstring(node->kind) << endl;
	depth++;
	for (Expression* expression: node->expressions)
	{
		expression->Accept(this);
	}
	depth--;
}

void TreeViewer::Visit(ConditionalExpression* node)
{
	Indent();
	wcout << expression_kind_to_wstring(node->kind) << endl;
	depth++;
	node->test->Accept(this);
	node->ifTrue->Accept(this);
	depth--;
}

void TreeViewer::Visit(FullConditionalExpression* node)
{
	Indent();
	wcout << expression_kind_to_wstring(node->kind) << endl;
	depth++;
	node->test->Accept(this);
	node->ifTrue->Accept(this);
	node->ifFalse->Accept(this);
	depth--;
}

void TreeViewer::Visit(ParameterExpression* node)
{
	Indent();
	wcout << node->name << endl;
}

void TreeViewer::Visit(CallExpression* node)
{
	Indent();
	depth++;
	node->procedure->Accept(this);
	for (unsigned int i = 0; i < node->arguments.size(); i++)
	{
		node->arguments.at(i)->Accept(this);
	}
	depth--;
}

void TreeViewer::Visit(WhileExpression* node)
{
	Indent();
	depth++;
	node->condition->Accept(this);
	node->body->Accept(this);
	depth--;
}

void TreeViewer::Visit(VarExpression* node)
{
	Indent();
	wcout << node->name << endl;
	depth++;
	node->value->Accept(this);
	depth--;
}

void TreeViewer::Visit(DefaultExpression* node)
{
	Indent();
	wcout << L"default(" << node->type->ToString() << L")" << endl;
}

void TreeViewer::Visit(DefineExpression* node)
{
	Indent();
	wcout << L"def" << endl;
}

void TreeViewer::Visit(NewExpression* node)
{
	Indent();
	wcout << L"new" << endl;
}

void TreeViewer::Indent()
{
	for (int i = 0; i < depth; i++)
	{
		wcout << L"    ";
	}
}
