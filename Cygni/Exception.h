#ifndef EXCEPTION_H
#define EXCEPTION_H

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
};

#endif // EXCEPTION_H 
