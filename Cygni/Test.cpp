#include "Test.h"
#include "Compiler.h"
#include "DebugInfo.h"
#include "Lexer.h"
#include "Parser.h"
#include "Scope.h"
#include "StringBuilder.h"
#include "Token.h"
#include "Type.h"
#include "TypeChecker.h"
#include "Visitor.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
	string path = "./TestCases/example3.txt";
	Lexer lexer(path);
	vector<Token> tokens = lexer.ReadAll();
	wcout << "tokens: " << endl;
    for (Token& t : tokens)
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

void AppendBytes(vector<byte>& code, byte* bytes, int length)
{
	for (int i = 0; i < length; i++)
	{
		code.push_back(bytes[i]);
	}
}

void AppendUShort(vector<byte>& code, unsigned short x)
{
    AppendBytes(code, reinterpret_cast<byte*>(&x), 2);
}

void TestCompiler()
{
	string path = "./TestCases/factorial.txt";
	Lexer lexer(path);
	vector<Token> tokens = lexer.ReadAll();
	wcout << "*** Lexical Analysis ***" << endl;
    for (Token& t : tokens)
	{
		t.Display();
	}

	DebugInfo debugInfo;
	Parser parser(tokens, debugInfo);
	Expression* program;

	wcout << "*** Syntax Analysis ***" << endl;
	try
	{
		program = parser.Program();
	}
	catch (SyntaxException& ex)
	{
		wcout << "(" << ex.line << ", " << ex.column << ")";
		wcout << ex.message << endl;
		throw ex;
	}

	TreeViewer viewer;
	program->Accept(&viewer);

	LocationRecord record;
	wcout << "*** Type Analysis ***" << endl;
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

	wcout << "*** Compilation ***" << endl;
	try
	{
		Compiler compiler(debugInfo, record);
		program->Accept(&compiler);
        vector<byte> code = compiler.GetByteSequence();

		ofstream file("./TestCases/factorial.bin", ofstream::binary);
		if (file)
		{
            for (byte item : code)
            {
                file.write(reinterpret_cast<char*>(&item), sizeof(byte));
            }
            file.close();
        }
        else
        {
            throw L"cannot write binary file";
        }
    }
    catch (SemanticException& ex)
    {
        wcout << ex.message << endl;
        throw ex;
    }
    catch (CompilationException& ex)
    {
        wcout << ex.message << endl;
        throw ex;
    }
    catch (const wchar_t* ex)
    {
        wcout << ex << endl;
        throw ex;
    }
}
