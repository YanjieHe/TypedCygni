#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "DebugInfo.h"
#include <string>

using std::wstring;

class Exception
{
  public:
    Exception();
};

class LexicalException
{
  public:
	int line;
	int column;
	wstring message;
	LexicalException(int line, int column, wstring message);
};

class SyntaxException
{
  public:
	int line;
	int column;
	wstring message;
	SyntaxException(int line, int column, wstring message);
};

class SemanticException
{
  public:
	int line;
	int column;
	wstring message;
	SemanticException(int line, int column, wstring message);
	SemanticException(const Position& position, wstring message);
};

class CompilationException
{
public:
	int line;
	int column;
	wstring message;
	CompilationException(int line, int column, wstring message);
	CompilationException(const Position& position, wstring message);
};

#endif // EXCEPTION_H 
