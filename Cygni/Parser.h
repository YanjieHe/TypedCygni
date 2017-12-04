#ifndef PARSER_H
#define PARSER_H

#include "Exception.h"
#include "Token.h"
#include "Expression.h"
#include "DebugInfo.h"
#include <vector>

using std::vector;

class Parser
{
public:
    Parser(vector<Token>& tokens, DebugInfo& debugInfo);
	Expression* Program();

private:
	vector<Token>& tokens;
	int tokens_pointer;
	DebugInfo& debugInfo;
	
	
	void Advance();
	void Back();

	Token& Current();
	int Line();
	int Column();

	bool IsEof();
	void Match(TokenKind kind);
	void Record(int line, int column, Expression* expression);

	Expression* Block();
	Expression* Statement();
	Expression* Assign();
	Expression* Or();
	Expression* And();
	Expression* Equality();
	Expression* Relation();
	Expression* Addition();
	Expression* Multiplication();
	Expression* Unary();
	Expression* Postfix();
	Expression* Factor();

	Type* ParseType();
	bool IsBasicType(wstring name);

	Expression* Var();
	Expression* Define();
	ParameterExpression* Parameter();
	Expression* If();
	Expression* While();
	Expression* Return();
};

#endif // PARSER_H 
