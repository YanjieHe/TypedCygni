#include "Lexer.hpp"
#include <iostream>

Lexer::Lexer(String code) : code{std::move(code)}, line{1}, column{1}, offset{0}
{
}

std::vector<Token> Lexer::ReadAll()
{
    std::vector<Token> tokens;
    static String opChars = "+-*/%><=!()[]{}:,.;";
    SkipWhitespaces();
    while (!IsEof())
    {
        if (IsDigit(Peek()))
        {
            tokens.push_back(ReadInt());
        }
        else if (Peek() == '"' || Peek() == '\'')
        {
            tokens.push_back(ReadString());
        }
        else if (IsLetter(Peek()) || Peek() == '_')
        {
            tokens.push_back(ReadIdentifier());
        }
        else if (opChars.find(Peek()) != String::npos)
        {
            tokens.push_back(ReadOperator());
        }
        else
        {
            throw LexicalException(line, column, "unsupported token");
        }
        SkipWhitespaces();
    }
    tokens.emplace_back(line, column, Tag::Eof, "<EOF>");
    return tokens;
}

std::unordered_map<String, Tag> Lexer::operators = {
        {"+",  Tag::Add},
        {"-",  Tag::Subtract},
        {"*",  Tag::Multiply},
        {"/",  Tag::Divide},
        {"%",  Tag::Modulo},
        {">",  Tag::GreaterThan},
        {"<",  Tag::LessThan},
        {">=", Tag::GreaterThanOrEqual},
        {"<=", Tag::LessThanOrEqual},
        {"==", Tag::Equal},
        {"!=", Tag::NotEqual},
        {"(",  Tag::LeftParenthesis},
        {")",   Tag::RightParenthesis},
        {"[",  Tag::LeftBracket},
        {"]",  Tag::RightBracket},
        {"{",  Tag::LeftBrace},
        {"}",  Tag::RightBrace},
        {":",  Tag::Colon},
        {",",  Tag::Comma},
        {".",  Tag::Dot},
        {";",  Tag::Semicolon},
        {"=",  Tag::Assign},
        {"->", Tag::RightArrow},
        {"<:", Tag::UpperBound},
        {":>", Tag::LowerBound}
};

Token Lexer::ReadInt()
{
    Reset();
    while (!IsEof() && IsDigit(Peek()))
    {
        Consume();
    }
    if (IsEof())
    {
        return Token(line, column, Tag::Integer, builder);
    }
    else
    {
        if (Peek() == '.')
        {
            Consume();
            return ReadFloat();
        }
        else
        {
            return Token(line, column, Tag::Integer, builder);
        }
    }
}

Token Lexer::ReadFloat()
{
    while (!IsEof() && IsDigit(Peek()))
    {
        Consume();
    }
    if (IsEof())
    {
        return Token(line, column, Tag::Float, builder);
    }
    else
    {
        if (Peek() == 'E' || Peek() == 'e')
        {
            Consume();
            if (Peek() == '+' || Peek() == '-')
            {
                Consume();
            }
            if (IsEof() || !IsDigit(Peek()))
            {
                throw LexicalException(line, column, "float literal");
            }
            else
            {
                while (!IsEof() && IsDigit(Peek()))
                {
                    Consume();
                }
                return Token(line, column, Tag::Float, builder);
            }
        }
        else
        {
            return Token(line, column, Tag::Float, builder);
        }
    }
}

Token Lexer::ReadString()
{
    Reset();
    Forward();
    while (!IsEof() && Peek() != '"')
    {
        if (Peek() == '\\')
        {
            Forward();
            if (IsEof())
            {
                throw LexicalException(line, column, "string literal");
            }
            else
            {
                builder.push_back(UnescapedChar(Peek()));
                Forward();
            }
        }
        else
        {
            Consume();
        }
    }
    if (IsEof())
    {
        throw LexicalException(line, column, "string literal");
    }
    else
    {
        Forward();
        return Token(line, column, Tag::String, builder);
    }
}

uint16_t Lexer::UnescapedChar(uint16_t c)
{
    switch (c)
    {
        case 'b':
            return '\b';
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case 'r':
            return '\r';
        case 'f':
            return '\f';
        case '\"':
            return '\"';
        case '\'':
            return '\'';
        case '\\':
            return '\\';
        default:
            throw LexicalException(line, column, "unsupported escaped character");
    }
}

Token Lexer::ReadIdentifier()
{
    Reset();
    Consume();
    while (!IsEof() && IsIdentifierChar(Peek()))
    {
        Consume();
    }
    return Token(line, column, Tag::Identifier, builder);
}

Token Lexer::ReadOperator()
{
    Char c1 = Peek();
    String s1;
    s1.push_back(c1);
    Forward();
    if (IsEof())
    {
        if (operators.find(s1) != operators.end())
        {
            return Token(line, column, operators[s1], s1);
        }
        else
        {
            throw LexicalException(line, column, "operator literal");
        }
    }
    else
    {
        Char c2 = Peek();
        String s12;
        s12.push_back(c1);
        s12.push_back(c2);
        if (operators.find(s12) != operators.end())
        {
            Forward();
            return Token(line, column, operators[s12], s12);
        }
        else if (operators.find(s1) != operators.end())
        {
            return Token(line, column, operators[s1], s1);
        }
        else
        {
            throw LexicalException(line, column, "operator literal");
        }
    }
}

void Lexer::SkipWhitespaces()
{
    while (!IsEof() && IsWhiteSpace(Peek()))
    {
        Forward();
    }
}