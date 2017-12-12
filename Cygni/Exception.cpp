#include "Exception.h"

Exception::Exception()
{
}

LexicalException::LexicalException(int line, int column, wstring message)
	:line{line}, column{column}, message{message}
{
}

SyntaxException::SyntaxException(int line, int column, wstring message)
	:line{line}, column{column}, message{message}
{
}

SemanticException::SemanticException(int line, int column, wstring message)
	:line{line}, column{column}, message{message}
{
}

SemanticException::SemanticException(const Position& position, wstring message)
	:line{position.line}, column{position.column}, message{message}
{
}

CompilationException::CompilationException(int line, int column, wstring message)
	:line{line}, column{column}, message{message}
{
}

CompilationException::CompilationException(const Position& position, wstring message)
	:line{position.line}, column{position.column}, message{message}
{
}
