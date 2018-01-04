#ifndef LEXER_H
#define LEXER_H

#include "Exception.h"
#include "StringBuilder.h"
#include "Token.h"
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::wifstream;

class Lexer
{
public:
    Lexer(string path);
	vector<Token> ReadAll();

private:
    wifstream stream;
    StringBuilder builder;

	int line;
	int column;

	int Peek();
	wchar_t Read();
	bool IsEof();
	void Consume();
	void Move();

	Token Digits();
	Token Decimals();
	Token Exponent();
	Token Text();
	Token Identifier();
	Token Operator();

	void EscapedChar();
	void SkipSpaces();
	void SetPosition(int currentLine, int currentColumn, Token& token);
};

#endif // LEXER_H
