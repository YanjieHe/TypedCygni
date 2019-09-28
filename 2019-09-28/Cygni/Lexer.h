#ifndef LEXER_H
#define LEXER_H

#include "Exception.h"
#include "Token.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::wifstream;
using std::wostringstream;

class Lexer
{
public:
    Lexer(string path);
    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;
	vector<Token> ReadAll();

private:
    wifstream stream;
    wostringstream builder;

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
    void Clear();
};

#endif // LEXER_H
