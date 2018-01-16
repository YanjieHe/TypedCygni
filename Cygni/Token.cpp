#include "Token.h"
#include <iostream>

using namespace std;

Token::Token(int line, int column, TokenKind kind, wstring text)
    : line{line}, column{column}, kind{kind}, text{text}
{
	CheckKeywords();
}

Token::Token(TokenKind kind, wstring text) : Token(-1, -1, kind, text)
{
}

void Token::Display()
{
	wcout << L"(" << line << L", " << column << "): ";
	wcout << L"[" << TokenKindToString(kind);
	wcout << L", " << text << L"]" << endl;
}

void Token::CheckKeywords()
{
    if (this->kind == TokenKind::Name)
    {
        if (table.find(text) != table.end())
        {
            this->kind = table[text];
        }
    }
}

map<wstring, TokenKind> Token::table = {
    {L"and", TokenKind::And},		{L"or", TokenKind::Or},
    {L"not", TokenKind::Not},		{L"true", TokenKind::True},
    {L"false", TokenKind::False},   {L"var", TokenKind::Var},
    {L"def", TokenKind::Define},	{L"if", TokenKind::If},
    {L"else", TokenKind::Else},		{L"while", TokenKind::While},
    {L"return", TokenKind::Return}, {L"import", TokenKind::Import}};
