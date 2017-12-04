#include "Test.h"
#include "StringBuilder.h"
#include "Token.h"
#include "Lexer.h"
#include "Type.h"
#include "Parser.h"
#include "DebugInfo.h"
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

	vector<Type*>* params = new vector<Type*>(2);
	params->at(0) = Type::Int();
	params->at(1) = Type::Int();
	FunctionType ft(params, Type::Int());
	wcout << ft.ToString() << endl;

	ArrayType atype(Type::String());
	wcout << atype.ToString() << endl;
}

void TestParser()
{
	// TO DO
}
