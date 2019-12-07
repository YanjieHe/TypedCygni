#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "Position.h"
#include <string>

using std::wstring;

class Exception
{
public:
    wstring message;
    Exception(wstring name);
};

class CodeException : public Exception
{
public:
    int line;
    int column;
    CodeException(int line, int column, wstring message);
};

class LexicalException : public CodeException
{
public:
	LexicalException(int line, int column, wstring message);
};

class SyntaxException : public CodeException
{
public:
	SyntaxException(int line, int column, wstring message);
    SyntaxException(const Position& position, wstring message);
};

class SemanticException : public CodeException
{
public:
	SemanticException(int line, int column, wstring message);
	SemanticException(const Position& position, wstring message);
};

class CompilationException : public CodeException
{
public:
	CompilationException(int line, int column, wstring message);
	CompilationException(const Position& position, wstring message);
};

class NotImplementedException : public Exception
{
public:
    NotImplementedException();
    NotImplementedException(wstring message);
};

class FileNotFoundException : public Exception
{
public:
    FileNotFoundException();
    FileNotFoundException(wstring message);
};
#endif // EXCEPTION_H
