package cygni.lexer;

public enum Tag {
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

    Eof
}
