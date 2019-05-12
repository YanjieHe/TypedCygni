#include "Parser.hpp"

ParserException::ParserException(int line, int column, String message)
        : line{line}, column{column}, message{std::move(message)}
{

}


Parser::Parser(String path, Vector<Token> tokens)
        : path{std::move(path)}, tokens{std::move(tokens)}, offset{0}
{
}

Program Parser::ParseProgram()
{
    Vector<Ptr<DefClass>> classes;
    Vector<Ptr<DefModule>> modules;
    while (!IsEof())
    {
        if (Look().tag == Tag::Class)
        {
            classes.push_back(ParseDefClass());
        }
        else
        {
            modules.push_back(ParseDefModule());
        }
    }
    return Program(path, classes, modules);
}

Ptr<Ast> Parser::Statement()
{
    if (Look().tag == Tag::If)
    {
        return IfStatement();
    }
    else if (Look().tag == Tag::Var)
    {
        return ParseVar();
    }
    else if (Look().tag == Tag::Def)
    {
        return ParseDef();
    }
    else if (Look().tag == Tag::Return)
    {
        return ParseReturn();
    }
    else if (Look().tag == Tag::While)
    {
        return ParseWhile();
    }
    else
    {
        return ParseAssign();
    }
}

Ptr<Ast> Parser::ParseAssign()
{
    auto &start = Look();
    Ptr<Ast> x = ParseOr();
    if (Look().tag == Tag::Assign)
    {
        Token t = Match(Tag::Assign);
        Ptr<Ast> y = ParseOr();
        return New<Assign>(GetPos(start), x, y);
    }
    else
    {
        return x;
    }
}

Ptr<Ast> Parser::ParseOr()
{
    auto &start = Look();
    Ptr<Ast> x = ParseAnd();
    while (Look().tag == Tag::Or)
    {
        Token t = Match(Tag::Or);
        Ptr<Ast> y = ParseAnd();
        x = New<Or>(GetPos(start), x, y);
    }
    return x;
}

Ptr<Ast> Parser::ParseAnd()
{
    auto &start = Look();
    Ptr<Ast> x = ParseEquality();
    while (Look().tag == Tag::And)
    {
        Token t = Match(Tag::And);
        Ptr<Ast> y = ParseEquality();
        x = New<And>(GetPos(start), x, y);
    }
    return x;
}

Ptr<Ast> Parser::ParseEquality()
{
    auto &start = Look();
    Ptr<Ast> x = ParseRelation();
    while (Look().tag == Tag::Equal || Look().tag == Tag::NotEqual)
    {
        Token t = Look();
        Move();
        Ptr<Ast> y = ParseRelation();
        if (t.tag == Tag::Equal)
        {
            x = New<Equal>(GetPos(start), x, y);
        }
        else
        {
            x = New<NotEqual>(GetPos(start), x, y);
        }
    }
    return x;
}

Ptr<Ast> Parser::ParseRelation()
{
    auto &start = Look();
    Ptr<Ast> x = ParseExpr();
    if (Look().tag == Tag::GreaterThan
        || Look().tag == Tag::LessThan
        || Look().tag == Tag::GreaterThanOrEqual
        || Look().tag == Tag::LessThanOrEqual)
    {
        Token t = Look();
        Move();
        Ptr<Ast> y = ParseExpr();
        if (t.tag == Tag::GreaterThan)
        {
            return New<GreaterThan>(GetPos(start), x, y);
        }
        else if (t.tag == Tag::LessThan)
        {
            return New<LessThan>(GetPos(start), x, y);
        }
        else if (t.tag == Tag::GreaterThanOrEqual)
        {
            return New<GreaterThanOrEqual>(GetPos(start), x, y);
        }
        else
        {
            return New<LessThanOrEqual>(GetPos(start), x, y);
        }
    }
    else
    {
        return x;
    }
}

Ptr<Ast> Parser::ParseExpr()
{
    auto &start = Look();
    Ptr<Ast> x = ParseTerm();
    while (Look().tag == Tag::Add || Look().tag == Tag::Subtract)
    {
        Token t = Look();
        Move();
        Ptr<Ast> y = ParseTerm();
        if (t.tag == Tag::Add)
        {
            x = New<Add>(GetPos(start), x, y);
        }
        else
        {
            x = New<Subtract>(GetPos(start), x, y);
        }
    }
    return x;
}

Ptr<Ast> Parser::ParseTerm()
{
    auto &start = Look();
    Ptr<Ast> x = ParseUnary();
    while (Look().tag == Tag::Multiply || Look().tag == Tag::Divide)
    {
        Token t = Look();
        Move();
        Ptr<Ast> y = ParseUnary();
        if (t.tag == Tag::Multiply)
        {
            x = New<Multiply>(GetPos(start), x, y);
        }
        else
        {
            x = New<Divide>(GetPos(start), x, y);
        }
    }
    return x;
}

Ptr<Ast> Parser::ParseUnary()
{
    auto &start = Look();
    if (Look().tag == Tag::Add)
    {
        Move();
        Ptr<Ast> x = ParseUnary();
        return New<UnaryPlus>(GetPos(start), x);
    }
    else if (Look().tag == Tag::Subtract)
    {
        Move();
        Ptr<Ast> x = ParseUnary();
        return New<UnaryMinus>(GetPos(start), x);
    }
    else if (Look().tag == Tag::Not)
    {
        Move();
        Ptr<Ast> x = ParseUnary();
        return New<Not>(GetPos(start), x);
    }
    else
    {
        return ParsePostfix();
    }
}

Ptr<Ast> Parser::ParsePostfix()
{
    Ptr<Ast> x = ParseFactor();
    while (Look().tag == Tag::LeftParenthesis || Look().tag == Tag::LeftBracket)
    {
        auto &start = Look();

        if (Look().tag == Tag::LeftParenthesis)
        {
            Vector<Ptr<Ast>> arguments;
            Match(Tag::LeftParenthesis);
            if (Look().tag == Tag::RightParenthesis)
            {
                Match(Tag::RightParenthesis);
                x = New<Call>(GetPos(start), x, arguments);
            }
            else
            {
                arguments.push_back(ParseOr());
                while (!IsEof() && Look().tag != Tag::RightParenthesis)
                {
                    Match(Tag::Comma);
                    arguments.push_back(ParseOr());
                }
                Match(Tag::RightParenthesis);
                x = New<Call>(GetPos(start), x, arguments);
            }
        }
        else if (Look().tag == Tag::LeftBracket)
        {
            // TO DO
        }
    }
    return x;
}

Ptr<Ast> Parser::ParseFactor()
{
    if (Look().tag == Tag::LeftParenthesis)
    {
        Move();
        Ptr<Ast> x = ParseOr();
        Match(Tag::RightParenthesis);
        return x;
    }
    else if (Look().tag == Tag::Integer
             || Look().tag == Tag::Float
             || Look().tag == Tag::String)
    {
        Tag tag = Look().tag;
        String v = Look().text;
        auto &start = Look();
        Move();
        Ptr<Ast> x;
        if (tag == Tag::Integer)
        {
            x = New<Constant>(GetPos(start), Constant::ConstantType::Int32Type, v);
        }
        else if (tag == Tag::Float)
        {
            x = New<Constant>(GetPos(start), Constant::ConstantType::FloatType, v);
        }
        else
        {
            x = New<Constant>(GetPos(start), Constant::ConstantType::StringType, v);
        }
        return x;
    }
    else if (Look().tag == Tag::True)
    {
        auto &start = Look();
        Move();
        Ptr<Ast> x = New<Constant>(GetPos(start), Constant::ConstantType::BooleanType, "true");
        return x;
    }
    else if (Look().tag == Tag::False)
    {
        auto &start = Look();
        Move();
        Ptr<Ast> x = New<Constant>(GetPos(start), Constant::ConstantType::BooleanType, "false");
        return x;
    }
    else if (Look().tag == Tag::Identifier)
    {
        String name = Look().text;
        auto &start = Look();
        Move();
        Ptr<Ast> x = New<Name>(GetPos(start), name);
        return x;
    }
    else
    {
        throw ParserException(Look().line, Look().column, "factor");
    }
}

Ptr<Ast> Parser::ParseBlock()
{
    auto &start = Look();
    Match(Tag::LeftBrace);
    Vector<Ptr<Ast>> expressions;
    while (!IsEof() && Look().tag != Tag::RightBrace)
    {
        expressions.push_back(Statement());
    }
    Match(Tag::RightBrace);
    return New<Block>(GetPos(start), expressions);
}

Ptr<Ast> Parser::IfStatement()
{
    auto &start = Look();
    Match(Tag::If);
    Ptr<Ast> condition = ParseOr();
    Ptr<Ast> ifTrue = ParseBlock();
    if (Look().tag == Tag::Else)
    {
        Match(Tag::Else);
        if (Look().tag == Tag::If)
        {
            Ptr<Ast> chunk = IfStatement();
            return New<IfElse>(GetPos(start), condition, ifTrue, chunk);
        }
        else
        {
            Ptr<Ast> chunk = ParseBlock();
            return New<IfElse>(GetPos(start), condition, ifTrue, chunk);
        }
    }
    else
    {
        return New<IfThen>(GetPos(start), condition, ifTrue);
    }
}

Ptr<Var> Parser::ParseVar()
{
    auto &start = Look();
    Match(Tag::Var);
    Token t = Match(Tag::Identifier);
    if (Look().tag == Tag::Colon)
    {
        Match(Tag::Colon);
        auto type = Optional<Ptr<Type>>(ParseType());
        if (Look().tag == Tag::Assign)
        {
            Match(Tag::Assign);
            auto value = Optional<Ptr<Ast>>(ParseOr());
            return New<Var>(GetPos(start), t.text, type, value);
        }
        else
        {
            return New<Var>(GetPos(start), t.text, type, Optional<Ptr<Ast>>());
        }
    }
    else
    {
        Match(Tag::Assign);
        auto value = Optional<Ptr<Ast>>(ParseOr());
        return New<Var>(GetPos(start), t.text, Optional<Ptr<Type>>(), value);
    }
}

Ptr<Var> Parser::ParseVarDeclaration()
{
    auto &start = Look();
    Match(Tag::Var);
    Token t = Match(Tag::Identifier);
    Match(Tag::Colon);
    auto type = Optional<Ptr<Type>>(ParseType());
    return New<Var>(GetPos(start), t.text, type, Optional<Ptr<Ast>>());
}

Ptr<Def> Parser::ParseDef()
{
    auto &start = Look();
    Match(Tag::Def);
    String name = Match(Tag::Identifier).text;

    Match(Tag::LeftParenthesis);
    Vector<Parameter> parameters;
    if (Look().tag != Tag::RightParenthesis)
    {
        parameters.push_back(ParseParameter());
        while (!IsEof() && Look().tag != Tag::RightParenthesis)
        {
            Match(Tag::Comma);
            parameters.push_back(ParseParameter());
        }
    }
    Match(Tag::RightParenthesis);
    Match(Tag::Colon);
    auto returnType = ParseType();
    Ptr<Ast> body = ParseBlock();
    return New<Def>(GetPos(start), name, parameters,
                    Def::MakeFunctionType(parameters, returnType), body);
}

Parameter Parser::ParseParameter()
{
    String name = Match(Tag::Identifier).text;
    Match(Tag::Colon);
    auto type = ParseType();
    return Parameter(name, type);
}

Ptr<Type> Parser::ParseType()
{
    String name = Match(Tag::Identifier).text;
    if (Look().tag == Tag::LeftBracket)
    {
        Vector<Ptr<Type>> types = ParseTypeArguments();
        auto result = New<TypeList>(name, types);
        if (result)
        {
            throw ParserException(Look().line, Look().column, "type error");
        }
        else
        {
            return result;
        }
    }
    else
    {
        return New<TypeLeaf>(name);
    }
}

Ptr<Ast> Parser::ParseReturn()
{
    auto &start = Look();
    Match(Tag::Return);
    Ptr<Ast> value = ParseOr();
    return New<Return>(GetPos(start), value);
}

Vector<Ptr<Type>> Parser::ParseTypeArguments()
{
    Match(Tag::LeftBracket);
    Vector<Ptr<Type>> types;
    types.push_back(ParseType());
    while (!IsEof() && Look().tag != Tag::RightBracket)
    {
        Match(Tag::Comma);
        types.push_back(ParseType());
    }
    Match(Tag::RightBracket);
    return types;
}

Ptr<While> Parser::ParseWhile()
{
    auto &start = Look();
    Token token = Match(Tag::While);
    Ptr<Ast> condition = ParseOr();
    Ptr<Ast> body = ParseBlock();
    return New<While>(GetPos(start), condition, body);
}

Ptr<DefClass> Parser::ParseDefClass()
{
    auto &start = Look();
    Match(Tag::Class);
    String name = Match(Tag::Identifier).text;
    Vector<Ptr<Var>> fields;
    Vector<Ptr<Def>> methods;
    Match(Tag::LeftBrace);
    while (!IsEof() && Look().tag != Tag::RightBrace)
    {
        Access access;
        if (Look().tag == Tag::Private)
        {
            access = Access::Private;
        }
        else if (Look().tag == Tag::Protected)
        {
            access = Access::Protected;
        }
        else
        {
            access = Access::Public;
        }

        if (Look().tag == Tag::Var)
        {
            // ParseVar field: Type
            auto field = ParseVarDeclaration();
            field->access = access;
            fields.push_back(field);
        }
        else if (Look().tag == Tag::Def)
        {
            // def method(args..) { }
            auto method = ParseDef();
            method->access = access;
            methods.push_back(method);
        }
        else
        {
            throw ParserException(Look().line, Look().column, "unexpected token");
        }
    }
    Match(Tag::RightBrace);
    return New<DefClass>(GetPos(start), name, fields, methods);
}

Ptr<DefModule> Parser::ParseDefModule()
{
    auto &start = Look();
    Match(Tag::Module);
    String name = Match(Tag::Identifier).text;
    Vector<Ptr<Var>> fields;
    Vector<Ptr<Def>> methods;
    Match(Tag::LeftBrace);
    while (!IsEof() && Look().tag != Tag::RightBrace)
    {
        Access access;
        if (Look().tag == Tag::Private)
        {
            access = Access::Private;
        }
        else if (Look().tag == Tag::Protected)
        {
            access = Access::Protected;
        }
        else
        {
            access = Access::Public;
        }

        if (Look().tag == Tag::Var)
        {
            // ParseVar field: Type
            auto field = ParseVarDeclaration();
            field->access = access;
            fields.push_back(field);
        }
        else if (Look().tag == Tag::Def)
        {
            // def method(args..) { }
            auto method = ParseDef();
            method->access = access;
            methods.push_back(method);
        }
        else
        {
            throw ParserException(Look().line, Look().column, "unexpected token");
        }
    }
    Match(Tag::RightBrace);
    return New<DefModule>(GetPos(start), name, fields, methods);
}

