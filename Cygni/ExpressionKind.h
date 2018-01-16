#ifndef EXPRESSIONKIND_H
#define EXPRESSIONKIND_H

#include <string>

using std::wstring;

enum class ExpressionKind
{
	Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    Power,

    UnaryPlus,
    Negate,
    Convert,

    And,
    Or,
    Not,

    Assign,

    Equal,
    NotEqual,
    GreaterThan,
    GreaterThanOrEqual,
    LessThan,
    LessThanOrEqual,

    Constant,
    Block,
    Parameter,
    Conditional,
    Unit,
    While,
    Break,
    Return,

    ReadArray,
    WriteArray,
    Call,

	Var,
	Define,
	Default,
    New,
    Import
};

wstring ExpressionKindToString(ExpressionKind kind);

#endif // EXPRESSIONKIND_H
