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
    Define,
    Assign,
	GoesTo,

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
	Then,
    Else,
    While,
	Do,
	End,

	For,
    Break,
    Return,

    Eof
};

std::wstring token_kind_to_wstring(TokenKind kind);

#endif // TOKENKIND_H 
