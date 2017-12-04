#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <vector>
#include <string>
#include "Exception.h"
#include "StringBuilder.h"
#include "Token.h"

using std::wifstream;
using std::vector;
using std::string;

class Lexer
{
  public:
    Lexer(string path);
	~Lexer();
	vector<Token> ReadAll();

  private:
	wifstream *stream;
	StringBuilder* builder;

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
