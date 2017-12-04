#include "Token.h"
#include <iostream>

using namespace std;

Token::Token(int line, int column, TokenKind kind, wstring text)
	:line{line}, column{column}, kind{kind}, text{text}
{
}

Token::Token(TokenKind kind, wstring text)
	:Token(-1, -1, kind, text)
{
}

void Token::Display()
{
	wcout << L"(" << line << L", " << column << "): ";
	wcout << L"[" << token_kind_to_wstring(kind);
	wcout << ", " << text << "]" << endl;
}
