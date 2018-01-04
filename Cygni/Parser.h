#ifndef PARSER_H
#define PARSER_H

#include "DebugInfo.h"
#include "Exception.h"
#include "Expression.h"
#include "Token.h"
#include <vector>

using std::vector;

class Parser
{
public:
    Parser(vector<Token>& tokens, DebugInfo& debugInfo);
	Expression* Program();

private:
	vector<Token>& tokens;
    unsigned long tokens_pointer;
	DebugInfo& debugInfo;

	void Advance();
	void Back();

	Token& Current();
	int Line();
	int Column();
    Position CurrentPosition();

	bool IsEof();
	void Match(TokenKind kind);
    void Record(Position position, Expression* expression);

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

    Type ParseType();

	Expression* Var();
	Expression* Define();
	ParameterExpression* Parameter();
	Expression* If();
	Expression* While();
	Expression* Return();
	Expression* Class();

    Expression* Require();
};

#endif // PARSER_H
