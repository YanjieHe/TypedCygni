#include "ExpressionKind.h"

wstring expression_kind_to_wstring(ExpressionKind kind)
{
	switch (kind)
    {
    case ExpressionKind::Add:
        return L"Add";
    case ExpressionKind::Subtract:
        return L"Subtract";
    case ExpressionKind::Multiply:
        return L"Multiply";
    case ExpressionKind::Divide:
        return L"Divide";
    case ExpressionKind::Modulo:
        return L"Modulo";
    case ExpressionKind::Power:
        return L"Power";
    case ExpressionKind::UnaryPlus:
        return L"UnaryPlus";
    case ExpressionKind::Negate:
        return L"Negate";
    case ExpressionKind::Convert:
        return L"Convert";
    case ExpressionKind::And:
        return L"And";
    case ExpressionKind::Or:
        return L"Or";
    case ExpressionKind::Not:
        return L"Not";
    case ExpressionKind::Assign:
        return L"Assign";
    case ExpressionKind::Equal:
        return L"Equal";
    case ExpressionKind::NotEqual:
        return L"NotEqual";
    case ExpressionKind::GreaterThan:
        return L"GreaterThan";
    case ExpressionKind::GreaterThanOrEqual:
        return L"GreaterThanOrEqual";
    case ExpressionKind::LessThan:
        return L"LessThan";
    case ExpressionKind::LessThanOrEqual:
        return L"LessThanOrEqual";
    case ExpressionKind::Constant:
        return L"Constant";
    case ExpressionKind::Block:
        return L"Block";
    case ExpressionKind::Parameter:
        return L"Parameter";
    case ExpressionKind::VariableDefinition:
        return L"VariableDefinition";
    case ExpressionKind::FunctionDefinition:
        return L"FunctionDefinition";
    case ExpressionKind::Conditional:
        return L"Conditional";
    case ExpressionKind::Unit:
        return L"Unit";
    case ExpressionKind::While:
        return L"While";
    case ExpressionKind::Break:
        return L"Break";
    case ExpressionKind::Return:
        return L"Return";
    case ExpressionKind::ReadArray:
        return L"ReadArray";
    case ExpressionKind::WriteArray:
        return L"WriteArray";
    case ExpressionKind::Var:
        return L"Var";
    case ExpressionKind::Define:
        return L"Define";
    case ExpressionKind::Default:
        return L"Default";
    case ExpressionKind::New:
        return L"New";
    case ExpressionKind::Call:
        return L"Call";
    default:
        throw L"ExpressionKind Error";
	}
}
