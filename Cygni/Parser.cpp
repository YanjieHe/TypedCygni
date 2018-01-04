#include "Parser.h"

Parser::Parser(vector<Token>& tokens, DebugInfo& debugInfo)
    : tokens{tokens}, tokens_pointer{0}, debugInfo{debugInfo}
{
}

Expression* Parser::Program()
{
	vector<Expression*> expressions;
	while (!IsEof())
	{
		expressions.push_back(Statement());
	}
	Expression* result = new BlockExpression(expressions);
    Record(Position(0, 1), result);
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
    return tokens.at(tokens_pointer);
}

inline int Parser::Line()
{
	return Current().line;
}

inline int Parser::Column()
{
    return Current().column;
}

Position Parser::CurrentPosition()
{
    return Position(Current().line, Current().column);
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

inline void Parser::Record(Position position, Expression* expression)
{
    debugInfo.Record(position, expression);
}

Expression* Parser::Block()
{
    Position position = CurrentPosition();

    Match(TokenKind::LeftBrace);
	vector<Expression*> expressions;
	while (Current().kind != TokenKind::RightBrace)
	{
		expressions.push_back(Statement());
	}
	Match(TokenKind::RightBrace);
	Expression* result = new BlockExpression(expressions);
    Record(position, result);
	return result;
}

Expression* Parser::Statement()
{
	switch (Current().kind)
	{
    case TokenKind::Var:
        return Var();
    case TokenKind::Define:
        return Define();
    case TokenKind::If:
        return If();
    case TokenKind::While:
        return While();
    case TokenKind::Return:
        return Return();
    default:
        return Assign();
	}
}

Expression* Parser::Assign()
{
	Expression* left = Or();
	if (Current().kind == TokenKind::Assign)
	{
        Position position = CurrentPosition();

		Advance();
		Expression* right = Or();
		if (left->kind == ExpressionKind::Parameter)
		{
            left = new AssignExpression(static_cast<ParameterExpression*>(left),
                                        right);
            Record(position, left);
		}
		else
		{
            throw SyntaxException(position, L"can't assign to left object");
		}
	}
	return left;
}

Expression* Parser::Or()
{
	Expression* left = And();
	while (Current().kind == TokenKind::Or)
	{
        Position position = CurrentPosition();

		Advance();
		Expression* right = And();
		left = new BinaryExpression(ExpressionKind::Or, left, right);
        Record(position, left);
	}
	return left;
}

Expression* Parser::And()
{
	Expression* left = Equality();
	while (Current().kind == TokenKind::And)
	{
        Position position = CurrentPosition();

		Advance();
		Expression* right = Equality();
		left = new BinaryExpression(ExpressionKind::And, left, right);
        Record(position, left);
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
        Position position = CurrentPosition();
		Advance();
		Expression* right = Relation();
		if (token.kind == TokenKind::Equal)
		{
			left = new BinaryExpression(ExpressionKind::Equal, left, right);
            Record(position, left);
		}
		else
		{
			left = new BinaryExpression(ExpressionKind::NotEqual, left, right);
            Record(position, left);
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
        Position position = CurrentPosition();
		Advance();
		Expression* right = Addition();
		switch (token.kind)
		{
        case TokenKind::GreaterThan:
            left =
                new BinaryExpression(ExpressionKind::GreaterThan, left, right);
            Record(position, left);
            break;
        case TokenKind::LessThan:
            left = new BinaryExpression(ExpressionKind::LessThan, left, right);
            Record(position, left);
            break;
        case TokenKind::GreaterThanOrEqual:
            left = new BinaryExpression(ExpressionKind::GreaterThanOrEqual,
                                        left, right);
            Record(position, left);
            break;
        default:
        case TokenKind::LessThanOrEqual:
            left = new BinaryExpression(ExpressionKind::LessThanOrEqual, left,
                                        right);
            Record(position, left);
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
        Position position = CurrentPosition();
		Advance();
		Expression* right = Multiplication();
		if (token.kind == TokenKind::Add)
		{
			left = new BinaryExpression(ExpressionKind::Add, left, right);
            Record(position, left);
		}
		else
		{
			left = new BinaryExpression(ExpressionKind::Subtract, left, right);
            Record(position, left);
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
        Position position = CurrentPosition();
		Advance();
		Expression* right = Unary();
		if (token.kind == TokenKind::Multiply)
		{
			left = new BinaryExpression(ExpressionKind::Multiply, left, right);
            Record(position, left);
		}
		else if (token.kind == TokenKind::Divide)
		{
			left = new BinaryExpression(ExpressionKind::Divide, left, right);
            Record(position, left);
		}
		else
		{
			left = new BinaryExpression(ExpressionKind::Modulo, left, right);
            Record(position, left);
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
        Position position = CurrentPosition();
		Advance();
		Expression* operand = Unary();
		if (token.kind == TokenKind::Add)
		{
            Expression* result =
                new UnaryExpression(ExpressionKind::UnaryPlus, operand);
            Record(position, result);
			return result;
		}
		else if (token.kind == TokenKind::Subtract)
		{
            Expression* result =
                new UnaryExpression(ExpressionKind::Negate, operand);
            Record(position, result);
			return result;
		}
		else
		{
            Expression* result =
                new UnaryExpression(ExpressionKind::Not, operand);
            Record(position, result);
			return result;
		}
	}
	return Postfix();
}

Expression* Parser::Postfix()
{
	Expression* left = Factor();
    //	Token& token = Current();
    Position position = CurrentPosition();
	while (Current().kind == TokenKind::LeftParenthesis)
	{
		Advance();
		vector<Expression*> arguments;
		if (Current().kind == TokenKind::RightParenthesis)
		{
			Advance();
			left = new CallExpression(left, arguments);
            Record(position, left);
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
            Record(position, left);
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
        Position position = CurrentPosition();

		Constant value(TypeTag::Int, Current().text); // TO DO: Int64
		Advance();
		Expression* result = new ConstantExpression(value);
        Record(position, result);
		return result;
	}
	else if (Current().kind == TokenKind::Float)
	{
        Position position = CurrentPosition();

		Constant value(TypeTag::Double, Current().text); // TO DO: Float32
		Advance();
		Expression* result = new ConstantExpression(value);
        Record(position, result);
		return result;
	}
	else if (Current().kind == TokenKind::String)
	{
        Position position = CurrentPosition();

		Constant value(TypeTag::String, Current().text);
		Advance();
		Expression* result = new ConstantExpression(value);
        Record(position, result);
		return result;
	}
	else if (Current().kind == TokenKind::Name)
	{
        Position position = CurrentPosition();

		wstring name = Current().text;
		Advance();
		Expression* result = new ParameterExpression(name);
        Record(position, result);
		return result;
	}
	else
	{
		throw SyntaxException(Line(), Column(),
                              L"syntax error: " +
                                  token_kind_to_wstring(Current().kind));
	}
}

Type Parser::ParseType()
{
	wstring name = Current().text;
	Match(TokenKind::Name);
    if (Type::IsBasicType(name))
	{
        return Type::FromString(name);
	}
	else if (name == L"Array")
	{
		Match(TokenKind::LeftBracket);
        Type element = ParseType();
        Match(TokenKind::RightBracket);
        return Type::Array(element);
	}
	else
	{
		throw L"not supported type name";
	}
}

Expression* Parser::Var()
{
    Position position = CurrentPosition();

	Match(TokenKind::Var);
	wstring name = Current().text;
	Match(TokenKind::Name);

	if (Current().kind == TokenKind::Colon)
	{
		Match(TokenKind::Colon);
        Type type = ParseType();
        Match(TokenKind::Assign);
        Expression* value = Or();
        Expression* result = new VarExpression(name, value);
        result->type = type;
        Record(position, result);
        return result;
	}
	else
	{
        Match(TokenKind::Assign);
		Expression* value = Or();
		Expression* result = new VarExpression(name, value);
        Record(position, result);
		return result;
	}
}

Expression* Parser::Define()
{
    Position position = CurrentPosition();

	Match(TokenKind::Define);
	wstring name = Current().text;
	Match(TokenKind::Name);
	Match(TokenKind::LeftParenthesis);

	vector<ParameterExpression*> parameters;

	if (Current().kind == TokenKind::RightParenthesis)
	{
		Match(TokenKind::RightParenthesis);
        Type returnType = ParseType();
		Expression* body = Block();

        vector<Type> parametersType;

        Type ft = Type::Function(parametersType, returnType);

		Expression* result = new DefineExpression(name, parameters, body, ft);
        Record(position, result);
		return result;
	}
	else
	{
		parameters.push_back(Parameter());
		while (Current().kind != TokenKind::RightParenthesis)
		{
			Match(TokenKind::Comma);
			parameters.push_back(Parameter());
		}
		Match(TokenKind::RightParenthesis);
		Match(TokenKind::Colon);
        Type returnType = ParseType();
		Expression* body = Block();

        vector<Type> parametersType;
        for (ParameterExpression* item : parameters)
		{
            parametersType.push_back(item->type);
		}
        Type ft = Type::Function(parametersType, returnType);
		Expression* result = new DefineExpression(name, parameters, body, ft);
        Record(position, result);
		return result;
	}
}

ParameterExpression* Parser::Parameter()
{
    Position position = CurrentPosition();

	wstring name = Current().text;
	Match(TokenKind::Name);
	Match(TokenKind::Colon);
    Type type = ParseType();
	ParameterExpression* result = new ParameterExpression(name, type);
    Record(position, result);
	return result;
}

Expression* Parser::If()
{
    Position position = CurrentPosition();

	Match(TokenKind::If);
	Expression* test = Or();
	Expression* ifTrue = Block();
	if (Current().kind == TokenKind::Else)
	{
		Advance();
		if (Current().kind == TokenKind::If)
		{
			Expression* ifFalse = If();
            Expression* result =
                new FullConditionalExpression(test, ifTrue, ifFalse);
            Record(position, result);
			return result;
		}
		else
		{
			Expression* ifFalse = Block();
            Expression* result =
                new FullConditionalExpression(test, ifTrue, ifFalse);
            Record(position, result);
			return result;
		}
	}
	else
	{
		Expression* result = new ConditionalExpression(test, ifTrue);
        Record(position, result);
		return result;
	}
}

Expression* Parser::While()
{
    Position position = CurrentPosition();

	Match(TokenKind::While);
	Expression* condition = Or();
	Expression* body = Block();
	Expression* result = new WhileExpression(condition, body);
    Record(position, result);
	return result;
}

Expression* Parser::Return()
{
    Position position = CurrentPosition();

	Match(TokenKind::Return);
	Expression* value = Or();
	Expression* result = new ReturnExpression(value);
    Record(position, result);
    return result;
}

// Expression* Parser::Require()
//{
//    Position position = CurrentPosition();
//    Match(TokenKind::Require);
//    return nullptr;
//}
