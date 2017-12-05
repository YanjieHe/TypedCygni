#ifndef TOKEN_H
#define TOKEN_H

#include "TokenKind.h"
#include <string>

using std::wstring;

class Token
{
  public:
	int line;
	int column;
	TokenKind kind;
	wstring text;

    Token(int line, int column, TokenKind kind, wstring text);
	Token(TokenKind kind, wstring text);

	void Display();
	void CheckKeywords();
};

#endif // TOKEN_H 
