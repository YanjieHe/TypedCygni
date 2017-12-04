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
