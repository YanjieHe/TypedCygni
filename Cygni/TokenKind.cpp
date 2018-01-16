#include "TokenKind.h"

std::wstring TokenKindToString(TokenKind kind)
{
    switch (kind)
    {
    case TokenKind::Name:
        return L"Name";
    case TokenKind::Integer:
        return L"Integer";
    case TokenKind::Float:
        return L"Float";
    case TokenKind::String:
        return L"String";
    case TokenKind::Add:
        return L"Add";
    case TokenKind::Subtract:
        return L"Subtract";
    case TokenKind::Multiply:
        return L"Multiply";
    case TokenKind::Divide:
        return L"Divide";
    case TokenKind::Modulo:
        return L"Modulo";
    case TokenKind::Power:
        return L"Power";
    case TokenKind::And:
        return L"And";
    case TokenKind::Or:
        return L"Or";
    case TokenKind::Not:
        return L"Not";
	case TokenKind::Var:
		return L"Var";
    case TokenKind::Define:
        return L"Define";
    case TokenKind::Assign:
        return L"Assign";
    case TokenKind::Equal:
        return L"Equal";
    case TokenKind::NotEqual:
        return L"NotEqual";
    case TokenKind::GreaterThan:
        return L"GreaterThan";
    case TokenKind::GreaterThanOrEqual:
        return L"GreaterThanOrEquarl";
    case TokenKind::LessThan:
        return L"LessThan";
    case TokenKind::LessThanOrEqual:
        return L"LessThanOrEqual";
    case TokenKind::LeftBrace:
        return L"LeftBrace";
    case TokenKind::RightBrace:
        return L"RightBrace";
    case TokenKind::LeftBracket:
        return L"LeftBracket";
    case TokenKind::RightBracket:
        return L"RightBracket";
    case TokenKind::LeftParenthesis:
        return L"LeftParenthesis";
    case TokenKind::RightParenthesis:
        return L"RightParenthesis";
    case TokenKind::Dot:
        return L"Dot";
    case TokenKind::Comma:
        return L"Comma";
    case TokenKind::Semicolon:
        return L"Semicolon";
    case TokenKind::Colon:
        return L"Colon";
    case TokenKind::True:
        return L"True";
    case TokenKind::False:
        return L"False";
    case TokenKind::If:
        return L"If";
    case TokenKind::Else:
        return L"Else";
    case TokenKind::While:
        return L"While";
    case TokenKind::For:
        return L"For";
    case TokenKind::Break:
        return L"Break";
    case TokenKind::Return:
        return L"Return";
    case TokenKind::Import:
        return L"Import";
    default:
    case TokenKind::Eof:
        return L"Eof";
    }
}
