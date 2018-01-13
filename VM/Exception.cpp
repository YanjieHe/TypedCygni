#include "Exception.h"

Exception::Exception(std::wstring message) : message{message}
{
}

CodeException::CodeException(int line, int column, std::wstring message)
    : Exception(message), line{line}, column{column}
{
}
LexicalException::LexicalException(int line, int column, wstring message)
    : CodeException(line, column, message)
{
}

SyntaxException::SyntaxException(int line, int column, wstring message)
    : CodeException(line, column, message)
{
}

SyntaxException::SyntaxException(const Position& position, std::wstring message)
    : CodeException(position.line, position.column, message)
{
}

SemanticException::SemanticException(int line, int column, wstring message)
    : CodeException(line, column, message)
{
}

SemanticException::SemanticException(const Position& position, wstring message)
    : CodeException(position.line, position.column, message)
{
}

CompilationException::CompilationException(int line, int column,
                                           wstring message)
    : CodeException(line, column, message)
{
}

CompilationException::CompilationException(const Position& position,
                                           wstring message)
    : CodeException(position.line, position.column, message)
{
}

NotImplementedException::NotImplementedException()
    : Exception(L"The method or operation is not implemented.")
{
}

NotImplementedException::NotImplementedException(std::wstring message)
    : Exception(message)
{
}

FileNotFoundException::FileNotFoundException()
    : Exception(L"Unable to find the specified file.")
{
}

FileNotFoundException::FileNotFoundException(std::wstring message)
    : Exception(message)
{
}
