#include "Parser.h"
#include <memory>
using std::make_shared;
using std::static_pointer_cast;

Parser::Parser(vector<Token>& tokens, DebugInfo& debugInfo)
    : tokens{tokens}, tokens_pointer{0}, debugInfo{debugInfo}
{
}

ExpressionPtr Parser::Program()
{
    vector<ExpressionPtr> expressions;
	while (!IsEof())
	{
        expressions.push_back(StatementOnTop());
	}
    ExpressionPtr result = make_shared<BlockExpression>(expressions);
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

inline void Parser::Record(Position position, ExpressionPtr expression)
{
    debugInfo.Record(position, expression);
}

ExpressionPtr Parser::Block()
{
    Position position = CurrentPosition();

    Match(TokenKind::LeftBrace);
    vector<ExpressionPtr> expressions;
	while (Current().kind != TokenKind::RightBrace)
	{
		expressions.push_back(Statement());
	}
	Match(TokenKind::RightBrace);
    ExpressionPtr result = make_shared<BlockExpression>(expressions);
    Record(position, result);
	return result;
}

ExpressionPtr Parser::Statement()
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

ExpressionPtr Parser::Assign()
{
    ExpressionPtr left = Or();
	if (Current().kind == TokenKind::Assign)
	{
        Position position = CurrentPosition();

		Advance();
        ExpressionPtr right = Or();
		if (left->kind == ExpressionKind::Parameter)
		{
            left = make_shared<AssignExpression>(
                static_pointer_cast<ParameterExpression>(left), right);
            Record(position, left);
		}
		else
		{
            throw SyntaxException(position, L"can't assign to left object");
		}
	}
	return left;
}

ExpressionPtr Parser::Or()
{
    ExpressionPtr left = And();
	while (Current().kind == TokenKind::Or)
	{
        Position position = CurrentPosition();

		Advance();
        ExpressionPtr right = And();
        left = make_shared<BinaryExpression>(ExpressionKind::Or, left, right);
        Record(position, left);
	}
	return left;
}

ExpressionPtr Parser::And()
{
    ExpressionPtr left = Equality();
	while (Current().kind == TokenKind::And)
	{
        Position position = CurrentPosition();

		Advance();
        ExpressionPtr right = Equality();
        left = make_shared<BinaryExpression>(ExpressionKind::And, left, right);
        Record(position, left);
	}
	return left;
}

ExpressionPtr Parser::Equality()
{
    ExpressionPtr left = Relation();
	while (Current().kind == TokenKind::Equal ||
           Current().kind == TokenKind::NotEqual)
	{
		Token& token = Current();
        Position position = CurrentPosition();
		Advance();
        ExpressionPtr right = Relation();
		if (token.kind == TokenKind::Equal)
		{
            left = make_shared<BinaryExpression>(ExpressionKind::Equal, left,
                                                 right);
            Record(position, left);
		}
		else
		{
            left = make_shared<BinaryExpression>(ExpressionKind::NotEqual, left,
                                                 right);
            Record(position, left);
		}
	}
	return left;
}

ExpressionPtr Parser::Relation()
{
    ExpressionPtr left = Addition();
	if (Current().kind == TokenKind::GreaterThan ||
		Current().kind == TokenKind::LessThan ||
		Current().kind == TokenKind::GreaterThanOrEqual ||
		Current().kind == TokenKind::LessThanOrEqual)
	{
		Token& token = Current();
        Position position = CurrentPosition();
		Advance();
        ExpressionPtr right = Addition();
		switch (token.kind)
		{
        case TokenKind::GreaterThan:
            left = make_shared<BinaryExpression>(ExpressionKind::GreaterThan,
                                                 left, right);
            Record(position, left);
            break;
        case TokenKind::LessThan:
            left = make_shared<BinaryExpression>(ExpressionKind::LessThan, left,
                                                 right);
            Record(position, left);
            break;
        case TokenKind::GreaterThanOrEqual:
            left = make_shared<BinaryExpression>(
                ExpressionKind::GreaterThanOrEqual, left, right);
            Record(position, left);
            break;
        default:
        case TokenKind::LessThanOrEqual:
            left = make_shared<BinaryExpression>(
                ExpressionKind::LessThanOrEqual, left, right);
            Record(position, left);
            break;
		}
	}
	return left;
}

ExpressionPtr Parser::Addition()
{
    ExpressionPtr left = Multiplication();
	while (Current().kind == TokenKind::Add ||
           Current().kind == TokenKind::Subtract)
	{
		Token& token = Current();
        Position position = CurrentPosition();
		Advance();
        ExpressionPtr right = Multiplication();
		if (token.kind == TokenKind::Add)
		{
            left =
                make_shared<BinaryExpression>(ExpressionKind::Add, left, right);
            Record(position, left);
		}
		else
		{
            left = make_shared<BinaryExpression>(ExpressionKind::Subtract, left,
                                                 right);
            Record(position, left);
		}
	}
	return left;
}

ExpressionPtr Parser::Multiplication()
{
    ExpressionPtr left = Unary();
	while (Current().kind == TokenKind::Multiply ||
           Current().kind == TokenKind::Divide ||
           Current().kind == TokenKind::Modulo)
	{
		Token& token = Current();
        Position position = CurrentPosition();
		Advance();
        ExpressionPtr right = Unary();
		if (token.kind == TokenKind::Multiply)
		{
            left = make_shared<BinaryExpression>(ExpressionKind::Multiply, left,
                                                 right);
            Record(position, left);
		}
		else if (token.kind == TokenKind::Divide)
		{
            left = make_shared<BinaryExpression>(ExpressionKind::Divide, left,
                                                 right);
            Record(position, left);
		}
		else
		{
            left = make_shared<BinaryExpression>(ExpressionKind::Modulo, left,
                                                 right);
            Record(position, left);
		}
	}
	return left;
}

ExpressionPtr Parser::Unary()
{
	if (Current().kind == TokenKind::Add ||
		Current().kind == TokenKind::Subtract ||
		Current().kind == TokenKind::Not)
	{
		Token& token = Current();
        Position position = CurrentPosition();
		Advance();
        ExpressionPtr operand = Unary();
		if (token.kind == TokenKind::Add)
		{
            ExpressionPtr result = make_shared<UnaryExpression>(
                ExpressionKind::UnaryPlus, operand);
            Record(position, result);
			return result;
		}
		else if (token.kind == TokenKind::Subtract)
		{
            ExpressionPtr result =
                make_shared<UnaryExpression>(ExpressionKind::Negate, operand);
            Record(position, result);
			return result;
		}
		else
		{
            ExpressionPtr result =
                make_shared<UnaryExpression>(ExpressionKind::Not, operand);
            Record(position, result);
			return result;
		}
	}
	return Postfix();
}

ExpressionPtr Parser::Postfix()
{
    ExpressionPtr left = Factor();
    //	Token& token = Current();
    Position position = CurrentPosition();
	while (Current().kind == TokenKind::LeftParenthesis)
	{
		Advance();
        vector<ExpressionPtr> arguments;
		if (Current().kind == TokenKind::RightParenthesis)
		{
			Advance();
            left = make_shared<CallExpression>(left, arguments);
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
            left = make_shared<CallExpression>(left, arguments);
            Record(position, left);
		}
	}
	return left;
}

ExpressionPtr Parser::Factor()
{
	if (Current().kind == TokenKind::LeftParenthesis)
	{
		Advance();
        ExpressionPtr value = Or();
		Match(TokenKind::RightParenthesis);
		return value;
	}
	else if (Current().kind == TokenKind::Integer)
	{
        Position position = CurrentPosition();

		Constant value(TypeTag::Int, Current().text); // TO DO: Int64
		Advance();
        ExpressionPtr result = make_shared<ConstantExpression>(value);
        Record(position, result);
		return result;
	}
	else if (Current().kind == TokenKind::Float)
	{
        Position position = CurrentPosition();

		Constant value(TypeTag::Double, Current().text); // TO DO: Float32
		Advance();
        ExpressionPtr result = make_shared<ConstantExpression>(value);
        Record(position, result);
		return result;
	}
	else if (Current().kind == TokenKind::String)
	{
        Position position = CurrentPosition();

		Constant value(TypeTag::String, Current().text);
		Advance();
        ExpressionPtr result = make_shared<ConstantExpression>(value);
        Record(position, result);
		return result;
	}
	else if (Current().kind == TokenKind::Name)
	{
        Position position = CurrentPosition();

		wstring name = Current().text;
		Advance();
        ExpressionPtr result = make_shared<ParameterExpression>(name);
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
    Position p = CurrentPosition();
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
        throw SyntaxException(p, L"not supported type name: " + name);
	}
}

ExpressionPtr Parser::Var()
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
        ExpressionPtr value = Or();
        ExpressionPtr result = make_shared<VarExpression>(name, value);
        result->type = type;
        Record(position, result);
        return result;
	}
	else
	{
        Match(TokenKind::Assign);
        ExpressionPtr value = Or();
        ExpressionPtr result = make_shared<VarExpression>(name, value);
        Record(position, result);
		return result;
	}
}

ExpressionPtr Parser::Define()
{
    Position position = CurrentPosition();

	Match(TokenKind::Define);
	wstring name = Current().text;
	Match(TokenKind::Name);
	Match(TokenKind::LeftParenthesis);

    vector<ParameterExpressionPtr> parameters;

	if (Current().kind == TokenKind::RightParenthesis)
	{
		Match(TokenKind::RightParenthesis);
        Match(TokenKind::Colon);
        Type returnType = ParseType();
        ExpressionPtr body = Block();

        vector<Type> parametersType;

        Type ft = Type::Function(parametersType, returnType);

        ExpressionPtr result =
            make_shared<DefineExpression>(name, parameters, body, ft);
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
        ExpressionPtr body = Block();

        vector<Type> parametersType;
        for (ParameterExpressionPtr item : parameters)
		{
            parametersType.push_back(item->type);
		}
        Type ft = Type::Function(parametersType, returnType);
        ExpressionPtr result =
            make_shared<DefineExpression>(name, parameters, body, ft);
        Record(position, result);
		return result;
	}
}

ParameterExpressionPtr Parser::Parameter()
{
    Position position = CurrentPosition();

	wstring name = Current().text;
	Match(TokenKind::Name);
	Match(TokenKind::Colon);
    Type type = ParseType();
    ParameterExpressionPtr result =
        make_shared<ParameterExpression>(name, type);
    Record(position, result);
	return result;
}

ExpressionPtr Parser::If()
{
    Position position = CurrentPosition();

	Match(TokenKind::If);
    ExpressionPtr test = Or();
    ExpressionPtr ifTrue = Block();
	if (Current().kind == TokenKind::Else)
	{
		Advance();
		if (Current().kind == TokenKind::If)
		{
            ExpressionPtr ifFalse = If();
            ExpressionPtr result =
                make_shared<FullConditionalExpression>(test, ifTrue, ifFalse);
            Record(position, result);
			return result;
		}
		else
		{
            ExpressionPtr ifFalse = Block();
            ExpressionPtr result =
                make_shared<FullConditionalExpression>(test, ifTrue, ifFalse);
            Record(position, result);
			return result;
		}
	}
	else
	{
        ExpressionPtr result = make_shared<ConditionalExpression>(test, ifTrue);
        Record(position, result);
		return result;
	}
}

ExpressionPtr Parser::While()
{
    Position position = CurrentPosition();

	Match(TokenKind::While);
    ExpressionPtr condition = Or();
    ExpressionPtr body = Block();
    ExpressionPtr result = make_shared<WhileExpression>(condition, body);
    Record(position, result);
	return result;
}

ExpressionPtr Parser::Return()
{
    Position position = CurrentPosition();

	Match(TokenKind::Return);
    ExpressionPtr value = Or();
    ExpressionPtr result = make_shared<ReturnExpression>(value);
    Record(position, result);
    return result;
}

ExpressionPtr Parser::Import()
{
    Position position = CurrentPosition();
    Match(TokenKind::Import);
    wstring name = Current().text;
    Match(TokenKind::Name);
    while (Current().kind == TokenKind::Dot)
    {
        Advance();
        name += Current().text;
        Match(TokenKind::Name);
    }
    ExpressionPtr result = make_shared<ImportExpression>(name);
    Record(position, result);
    return result;
}

ExpressionPtr Parser::StatementOnTop()
{
    switch (Current().kind)
    {
    case TokenKind::Var:
        return Var();
    case TokenKind::Define:
        return Define();
    case TokenKind::Import:
        return Import();
    default:
        throw SyntaxException(CurrentPosition(), L"erroneous statement");
    }
}

// ExpressionPtr Parser::Require()
//{
//    Position position = CurrentPosition();
//    Match(TokenKind::Require);
//    return nullptr;
//}
