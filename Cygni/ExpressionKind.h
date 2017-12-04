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
    VariableDefinition,
    FunctionDefinition,
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
	New
};

wstring expression_kind_to_wstring(ExpressionKind kind);

#endif // EXPRESSIONKIND_H 
