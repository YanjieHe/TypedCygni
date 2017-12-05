#include "Test.h"
#include "StringBuilder.h"
#include "Token.h"
#include "Lexer.h"
#include "Type.h"
#include "Parser.h"
#include "DebugInfo.h"
#include "Visitor.h"
#include "Scope.h"
#include "TypeChecker.h"
#include <vector>
#include <iostream>

using namespace std;

void TestStringBuilder()
{
	StringBuilder builder;
	wstring msg = L"今天好开心";
	builder.Append(msg);
	builder.Append(L'h');
	wcout << builder.ToString() << endl;
}

void TestLexer()
{
	string path = "./TestCases/example1.txt";
	Lexer lexer(path);
	vector<Token> tokens = lexer.ReadAll();
	wcout << "tokens: " << endl;
	for (Token& t: tokens)
	{
		t.Display();
	}
}

void TestType()
{
	wcout << Type::Int()->ToString() << endl;
	wcout << Type::Boolean()->ToString() << endl;

	vector<Type*> params(2);
	params.at(0) = Type::Int();
	params.at(1) = Type::Int();
	FunctionType ft(params, Type::Int());
	wcout << ft.ToString() << endl;

	ArrayType atype(Type::String());
	wcout << atype.ToString() << endl;
}

void TestParser()
{
	string path = "./TestCases/example1.txt";
	Lexer lexer(path);
	vector<Token> tokens = lexer.ReadAll();
	wcout << "tokens: " << endl;
	for (Token& t: tokens)
	{
		t.Display();
	}
	
	DebugInfo debugInfo;
	Parser parser(tokens, debugInfo);
	Expression* program = parser.Program();

	TreeViewer viewer;
	program->Accept(&viewer);
}

void TestScope()
{
	Scope* global = new GlobalScope();
	global->Define(L"a");
	global->Define(L"b");

	Scope* function = new FunctionScope(global);
	function->Define(L"a");
	function->Define(L"b");
	function->Define(L"c");

	wcout << global->Find(L"a").ToString() << endl;
	wcout << function->Find(L"a").ToString() << endl;
	wcout << global->Find(L"b").ToString() << endl;
	wcout << function->Find(L"c").ToString() << endl;
}

void TestTypeChecker()
{
	string path = "./TestCases/example2.txt";
	Lexer lexer(path);
	vector<Token> tokens = lexer.ReadAll();
	wcout << "tokens: " << endl;
	for (Token& t: tokens)
	{
		t.Display();
	}
	
	DebugInfo debugInfo;
	Parser parser(tokens, debugInfo);
	Expression* program = parser.Program();

	TreeViewer viewer;
	program->Accept(&viewer);

	LocationRecord record;

	try
	{
		TypeChecker checker(debugInfo, record);
		program->Accept(&checker);
	}
	catch (SemanticException& ex)
	{
		wcout << ex.message << endl;
		throw ex;
	}
}
