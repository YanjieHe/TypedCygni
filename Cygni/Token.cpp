#include "Token.h"
#include <iostream>

using namespace std;

Token::Token(int line, int column, TokenKind kind, wstring text)
	:line{line}, column{column}, kind{kind}, text{text}
{
	CheckKeywords();
}

Token::Token(TokenKind kind, wstring text)
	:Token(-1, -1, kind, text)
{
}

void Token::Display()
{
	wcout << L"(" << line << L", " << column << "): ";
	wcout << L"[" << token_kind_to_wstring(kind);
	wcout << L", " << text << L"]" << endl;
}

void Token::CheckKeywords()
{
	if (this->kind == TokenKind::Name)
	{
		if (this->text == L"and")
		{
			this->kind = TokenKind::And;
		}
		else if (this->text == L"or")
		{
			this->kind = TokenKind::Or;
		}
		else if (this->text == L"not")
		{
			this->kind = TokenKind::Not;
		}
		else if (this->text == L"true")
		{
			this->kind = TokenKind::True;
		}
		else if (this->text == L"false")
		{
			this->kind = TokenKind::False;
		}
		else if (this->text == L"var")
		{
			this->kind = TokenKind::Var;
		}
		else if (this->text == L"def")
		{
			this->kind = TokenKind::Define;
		}
		else if (this->text == L"if")
		{
			this->kind = TokenKind::If;
		}
		else if (this->text == L"else")
		{
			this->kind = TokenKind::Else;
		}
		else if (this->text == L"while")
		{
			this->kind = TokenKind::While;
		}
		else if (this->text == L"return")
		{
			this->kind = TokenKind::Return;
		}
		else
		{
			return;
		}
	}
}
