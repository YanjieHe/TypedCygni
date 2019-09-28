#ifndef TOKEN_H
#define TOKEN_H

#include "TokenKind.h"
#include <map>
#include <string>

using std::map;
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

private:
    static map<wstring, TokenKind> table;
};
#endif // TOKEN_H
