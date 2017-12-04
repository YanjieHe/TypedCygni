#ifndef TOKENKIND_H
#define TOKENKIND_H

#include <string>

enum class TokenKind
{
	Name,
    Integer,
    Float,
    String,

    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    Power,

    And,
    Or,
    Not,

	Var,
    Define,
    Assign,

    Equal,
    NotEqual,
    GreaterThan,
    GreaterThanOrEqual,
    LessThan,
    LessThanOrEqual,

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

    True,
    False,

    If,
    Else,
    While,
    For,
    Break,
    Return,
    Function,

    Eof
};

std::wstring token_kind_to_wstring(TokenKind kind);

#endif // TOKENKIND_H 
