#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <unordered_map>
#include "String.hpp"

enum class Tag
{
    Identifier,
    Integer,
    Float,
    String,

    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,

    Equal,
    NotEqual,
    GreaterThan,
    GreaterThanOrEqual,
    LessThan,
    LessThanOrEqual,

    And,
    Or,
    Not,

    Var,
    Def,
    Assign,

    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    LeftParenthesis,
    RightParenthesis,

    Dot,
    Comma,
    Semicolon,
    Colon,
    RightArrow,

    True,
    False,

    If,
    Else,
    While,

    For,
    Break,
    Return,

    Import,
    Uses,

    LowerBound,
    UpperBound,

    Class,
    Private,
    Protected,
    Module,
    Eof
};

class Token
{
public:
    Tag tag;
    int line;
    int column;
    String text;

    Token(int line, int column, Tag tag, String text);

private:
    static std::unordered_map<String, Tag> keywords;
};

String TagToString(Tag tag);

#endif // TOKEN_HPP