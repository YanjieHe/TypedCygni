#include "Visitor.h"
#include "ExpressionKind.h"
#include <iostream>

using namespace std;

Visitor::~Visitor()
{
}

TreeViewer::TreeViewer() : depth{0}
{
}

void TreeViewer::Visit(UnaryExpression* node)
{
	Indent();
    PrintKind(node);
	depth++;
	node->operand->Accept(this);
	depth--;
}

void TreeViewer::Visit(BinaryExpression* node)
{
    Indent();
    PrintKind(node);
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
    PrintKind(node);
	depth++;
    for (ExpressionPtr expression : node->expressions)
	{
		expression->Accept(this);
	}
	depth--;
}

void TreeViewer::Visit(ConditionalExpression* node)
{
    Indent();
    PrintKind(node);
	depth++;
	node->test->Accept(this);
	node->ifTrue->Accept(this);
	depth--;
}

void TreeViewer::Visit(FullConditionalExpression* node)
{
    Indent();
    PrintKind(node);
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
    PrintKind(node);
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
    PrintKind(node);
	depth++;
	node->condition->Accept(this);
	node->body->Accept(this);
	depth--;
}

void TreeViewer::Visit(VarExpression* node)
{
    Indent();
    wcout << "var " << node->name << endl;
	depth++;
	node->value->Accept(this);
	depth--;
}

void TreeViewer::Visit(DefaultExpression* node)
{
	Indent();
    wcout << L"default(" << static_cast<Expression*>(node)->type.ToString()
          << L")" << endl;
}

void TreeViewer::Visit(DefineExpression* node)
{
	Indent();
    wcout << L"def " << node->name << endl;
    depth++;
    node->body->Accept(this);
    depth--;
}

void TreeViewer::Visit(NewExpression* node)
{
	Indent();
    PrintKind(node);
}

void TreeViewer::Visit(AssignExpression* node)
{
	Indent();
    PrintKind(node);
	depth++;
	node->variable->Accept(this);
	node->value->Accept(this);
	depth--;
}

void TreeViewer::Visit(ReturnExpression* node)
{
	Indent();
    PrintKind(node);
	depth++;
	node->value->Accept(this);
    depth--;
}

void TreeViewer::Visit(ImportExpression* node)
{
    wcout << expression_kind_to_wstring(node->kind) << L": " << node->name
          << endl;
}

void TreeViewer::Indent()
{
	for (int i = 0; i < depth; i++)
	{
		wcout << L"    ";
    }
}

void TreeViewer::PrintKind(Expression* node)
{
    wcout << expression_kind_to_wstring(node->kind) << endl;
}
