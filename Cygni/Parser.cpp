#include "Parser.h"

Parser::Parser(vector<Token>& tokens, DebugInfo& debugInfo)
	:tokens{tokens}, tokens_pointer{0}, debugInfo{debugInfo}
{
}

Expression* Parser::Program()
{
	int currentLine = Line();
	int currentColumn = Column();

	Match(TokenKind::LeftBrace);
	vector<Expression*> expressions;
	while (Current().kind != TokenKind::RightBrace)
	{
		expressions.push_back(Statement());
	}
	Match(TokenKind::RightBrace);
	Expression* result = new BlockExpression(expressions);
	Record(currentLine, currentColumn, result);
	return result;
}

inline void Parser::Advance()
{
	tokens_pointer++;
}

inline void Parser::Back()
{
	tokens_pointer--;
}

inline Token& Parser::Current()
{
	return tokens[tokens_pointer];
}

inline int Parser::Line()
{
	return Current().line;
}

inline int Parser::Column()
{
	return Current().column;
}

inline bool Parser::IsEof()
{
	return Current().kind == TokenKind::Eof;
}

void Parser::Match(TokenKind kind)
{
	if (Current().kind == kind)
	{
		Advance();
	}
	else
	{
		throw SyntaxException(Line(), Column(),
				L"expecting " + token_kind_to_wstring(kind) + 
				L", unexpected " +
				token_kind_to_wstring(Current().kind));
	}
}

inline void Parser::Record(int line, int column, Expression* expression)
{
	debugInfo.Record(line, column, expression);
}

Expression* Parser::Block()
{
	int currentLine = Line();
	int currentColumn = Column();

	Match(TokenKind::LeftBrace);
	vector<Expression*> expressions;
	while (Current().kind != TokenKind::RightBrace)
	{
		expressions.push_back(Statement());
	}
	Match(TokenKind::RightBrace);
	Expression* result = new BlockExpression(expressions);
	Record(currentLine, currentColumn, result);
}

Expression* Parser::Statement()
{
	return Assign();
}

Expression* Parser::Assign()
{
	Expression* left = Or();
	if (Current().kind == TokenKind::Assign)
	{
		int currentLine = Line();
		int currentColumn = Column();
		Advance();
		Expression* right = Or();
		left = new BinaryExpression(ExpressionKind::Assign, left, right);
		Record(currentLine, currentColumn, left);
	}
	return left;
}

Expression* Parser::Or()
{
	Expression* left = And();
	while (Current().kind == TokenKind::Or)
	{
		int currentLine = Line();
		int currentColumn = Column();
		Advance();
		Expression* right = And();
		left = new BinaryExpression(ExpressionKind::Or, left, right);
		Record(currentLine, currentColumn, left);
	}
	return left;
}

Expression* Parser::And()
{
	Expression* left = Equality();
	while (Current().kind == TokenKind::And)
	{
		int currentLine = Line();
		int currentColumn = Column();
		Advance();
		Expression* right = Equality();
		left = new BinaryExpression(ExpressionKind::And, left, right);
		Record(currentLine, currentColumn, left);
	}
	return left;
}

Expression* Parser::Equality()
{
	Expression* left = Relation();
	while (Current().kind == TokenKind::Equal ||
			Current().kind == TokenKind::NotEqual)
	{
		Token& token = Current();
		Advance();
		Expression* right = Relation();
		if (token.kind == TokenKind::Equal)
		{
			left = new BinaryExpression(ExpressionKind::Equal, left, right);
			Record(token.line, token.column, left);
		}
		else
		{
			left = new BinaryExpression(ExpressionKind::NotEqual, left, right);
			Record(token.line, token.column, left);
		}
	}
	return left;
}

Expression* Parser::Relation()
{
	Expression* left = Addition();
	if (Current().kind == TokenKind::GreaterThan ||
		Current().kind == TokenKind::LessThan ||
		Current().kind == TokenKind::GreaterThanOrEqual ||
		Current().kind == TokenKind::LessThanOrEqual)
	{
		Token& token = Current();
		Advance();
		Expression* right = Addition();
		switch (token.kind)
		{
			case TokenKind::GreaterThan:
				left = new BinaryExpression(ExpressionKind::GreaterThan, left, right);
				Record(token.line, token.column, left);
				break;
			case TokenKind::LessThan:
				left = new BinaryExpression(ExpressionKind::LessThan, left, right);
				Record(token.line, token.column, left);
				break;
			case TokenKind::GreaterThanOrEqual:
				left = new BinaryExpression(ExpressionKind::GreaterThanOrEqual, left, right);
				Record(token.line, token.column, left);
				break;
			default:
			case TokenKind::LessThanOrEqual:
				left = new BinaryExpression(ExpressionKind::LessThanOrEqual, left, right);
				Record(token.line, token.column, left);
				break;
		}
	}
	return left;
}

Expression* Parser::Addition()
{
	Expression* left = Multiplication();
	while (Current().kind == TokenKind::Add ||
			Current().kind == TokenKind::Subtract)
	{
		Token& token = Current();
		Advance();
		Expression* right = Multiplication();
		if (token.kind == TokenKind::Add)
		{
			left = new BinaryExpression(ExpressionKind::Add, left, right);
			Record(token.line, token.column, left);
		}
		else
		{
			left = new BinaryExpression(ExpressionKind::Subtract, left, right);
			Record(token.line, token.column, left);
		}
	}
	return left;
}

Expression* Parser::Multiplication()
{
	Expression* left = Unary();
	while (Current().kind == TokenKind::Multiply ||
			Current().kind == TokenKind::Divide ||
			Current().kind == TokenKind::Modulo)
	{
		Token& token = Current();
		Advance();
		Expression* right = Unary();
		if (token.kind == TokenKind::Multiply)
		{
			left = new BinaryExpression(ExpressionKind::Multiply, left, right);
			Record(token.line, token.column, left);
		}
		else if (token.kind == TokenKind::Divide)
		{
			left = new BinaryExpression(ExpressionKind::Divide, left, right);
			Record(token.line, token.column, left);
		}
		else
		{
			left = new BinaryExpression(ExpressionKind::Modulo, left, right);
			Record(token.line, token.column, left);
		}
	}
	return left;
}

Expression* Parser::Unary()
{
	if (Current().kind == TokenKind::Add ||
		Current().kind == TokenKind::Subtract ||
		Current().kind == TokenKind::Not)
	{
		Token& token = Current();
		Advance();
		Expression* operand = Unary();
		if (token.kind == TokenKind::Add)
		{
			Expression* result = new UnaryExpression(ExpressionKind::UnaryPlus, operand);
			Record(token.line, token.column, result);
			return result;
		}
		else if (token.kind == TokenKind::Subtract)
		{
			Expression* result = new UnaryExpression(ExpressionKind::Negate, operand);
			Record(token.line, token.column, result);
			return result;
		}
		else
		{
			Expression* result = new UnaryExpression(ExpressionKind::Not, operand);
			Record(token.line, token.column, result);
			return result;
		}
	}
	return Postfix();
}

Expression* Parser::Postfix()
{
	Expression* left = Factor();
	Token& token = Current();
	while (Current().kind == TokenKind::LeftParenthesis)
	{
		Advance();
		vector<Expression*> arguments;
		if (Current().kind == TokenKind::RightParenthesis)
		{
			Advance();
			left = new CallExpression(left, arguments);
			Record(token.line, token.column, left);
		}
		else
		{
			arguments.push_back(Or());
			while (Current().kind != TokenKind::RightParenthesis)
			{
				Match(TokenKind::Comma);
				arguments.push_back(Or());
			}
			Advance();
			left = new CallExpression(left, arguments);
			Record(token.line, token.column, left);
		}
	}
	return left;
}

Expression* Parser::Factor()
{
	if (Current().kind == TokenKind::LeftParenthesis)
	{
		Advance();
		Expression* value = Or();
		Match(TokenKind::RightParenthesis);
		return value;
	}
	else if (Current().kind == TokenKind::Integer)
	{
		int currentLine = Line();
		int currentColumn = Column();
		Constant value(TypeTag::Int, Current().text); // TO DO: Int64
		Advance();
		Expression* result = new ConstantExpression(value);
		Record(currentLine, currentColumn, result);
		return result;
	}
	else if (Current().kind == TokenKind::Float)
	{
		int currentLine = Line();
		int currentColumn = Column();
		Constant value(TypeTag::Double, Current().text); // TO DO: Float32
		Advance();
		Expression* result = new ConstantExpression(value);
		Record(currentLine, currentColumn, result);
		return result;
	}
	else if (Current().kind == TokenKind::String)
	{
		int currentLine = Line();
		int currentColumn = Column();
		Constant value(TypeTag::String, Current().text);
		Advance();
		Expression* result = new ConstantExpression(value);
		Record(currentLine, currentColumn, result);
		return result;
	}
	else if (Current().kind == TokenKind::Name)
	{
		int currentLine = Line();
		int currentColumn = Column();
		wstring name = Current().text;
		Advance();
		Expression* result = new ParameterExpression(name);
		Record(currentLine, currentColumn, result);
		return result;
	}
	else
	{
		throw SyntaxException(Line(), Column(),
				L"syntax error: " +
				token_kind_to_wstring(Current().kind));
	}
}
