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
    ExpressionPtr Program();

private:
	vector<Token>& tokens;
    unsigned int tokens_pointer;
	DebugInfo& debugInfo;

	void Advance();
	void Back();

	Token& Current();
	int Line();
	int Column();
    Position CurrentPosition();

	bool IsEof();
	void Match(TokenKind kind);
    void Record(Position position, ExpressionPtr expression);

    ExpressionPtr Block();
    ExpressionPtr Statement();
    ExpressionPtr Assign();
    ExpressionPtr Or();
    ExpressionPtr And();
    ExpressionPtr Equality();
    ExpressionPtr Relation();
    ExpressionPtr Addition();
    ExpressionPtr Multiplication();
    ExpressionPtr Unary();
    ExpressionPtr Postfix();
    ExpressionPtr Factor();

    Type ParseType();

    ExpressionPtr Var();
    ExpressionPtr Define();
    ParameterExpressionPtr Parameter();
    ExpressionPtr If();
    ExpressionPtr While();
    ExpressionPtr Return();
    ExpressionPtr Class();

    ExpressionPtr Import();
    ExpressionPtr StatementOnTop();
};

#endif // PARSER_H
