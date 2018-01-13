#include "Test.h"
#include "ByteCodeGenerator.h"
#include "Compiler.h"
#include "DebugInfo.h"
#include "Lexer.h"
#include "Parser.h"
#include "Scope.h"
#include "Token.h"
#include "Type.h"
#include "TypeChecker.h"
#include "Visitor.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void TestLexer()
{
	string path = "./TestCases/example1.txt";
	Lexer lexer(path);
	vector<Token> tokens = lexer.ReadAll();
	wcout << "tokens: " << endl;
    for (Token& t : tokens)
	{
		t.Display();
	}
}

void TestType()
{
    wcout << Type::Int().ToString() << endl;
    wcout << Type::Boolean().ToString() << endl;

    vector<Type> params = {Type::Int(), Type::Int()};
    Type ft = Type::Function(params, Type::Int());
	wcout << ft.ToString() << endl;

    Type atype = Type::Array(Type::String());
	wcout << atype.ToString() << endl;
}

void TestParser()
{
	string path = "./TestCases/example1.txt";
	Lexer lexer(path);
	vector<Token> tokens = lexer.ReadAll();
	wcout << "tokens: " << endl;
    for (Token& t : tokens)
	{
		t.Display();
	}

	DebugInfo debugInfo;
	Parser parser(tokens, debugInfo);
    ExpressionPtr program = parser.Program();

	TreeViewer viewer;
	program->Accept(&viewer);
}

void TestScope()
{
    ScopePtr global = make_shared<GlobalScope>(GlobalScope());
	global->Define(L"a");
	global->Define(L"b");

    FunctionScopePtr function = make_shared<FunctionScope>(global);
	function->Define(L"a");
	function->Define(L"b");
	function->Define(L"c");

	wcout << global->Find(L"a").ToString() << endl;
	wcout << function->Find(L"a").ToString() << endl;
	wcout << global->Find(L"b").ToString() << endl;
	wcout << function->Find(L"c").ToString() << endl;
}

void TestCompiler()
{
    string path = "./TestCases/factorial.txt";
    try
    {
        Compiler compiler(path);
        compiler.ParseAll();
        compiler.CheckType();
        compiler.Compile("./TestCases/factorial.bin");
    }
    catch (SyntaxException& ex)
    {
        wcout << "(" << ex.line << ", " << ex.column << "): ";
        wcout << ex.message << endl;
        throw ex;
    }
    catch (CompilationException& ex)
    {
        wcout << "(" << ex.line << ", " << ex.column << "): ";
        wcout << ex.message << endl;
        throw ex;
    }
}
