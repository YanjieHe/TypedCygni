#include "Token.hpp"
#include "Encoding.hpp"
#include <iostream>

Token::Token(int line, int column, Tag tag, String text)
        : line{line}, column{column}, tag{tag}, text{std::move(text)}
{
    if (tag == Tag::Identifier && keywords.find(this->text) != keywords.end())
    {
        this->tag = keywords[this->text];
    }
}

std::unordered_map<String, Tag> Token::keywords = {
        {"if",        Tag::If},
        {"else",      Tag::Else},
        {"var",       Tag::Var},
        {"def",       Tag::Def},
        {"return",    Tag::Return},
        {"true",      Tag::True},
        {"false",     Tag::False},
        {"while",     Tag::While},
        {"class",     Tag::Class},
        {"private",   Tag::Private},
        {"module",    Tag::Module},
        {"protected", Tag::Protected}
};

String TagToString(Tag tag)
{
    switch (tag)
    {
        case Tag::Identifier:
            return "Identifier";
        case Tag::Integer:
            return "Integer";
        case Tag::Float:
            return "Float";
        case Tag::String:
            return "String";
        case Tag::Add:
            return "Add";
        case Tag::Subtract:
            return "Subtract";
        case Tag::Multiply:
            return "Multiply";
        case Tag::Divide:
            return "Divide";
        case Tag::Modulo:
            return "Modulo";
        case Tag::Equal:
            return "Equal";
        case Tag::NotEqual:
            return "NotEqual";
        case Tag::GreaterThan:
            return "GreaterThan";
        case Tag::GreaterThanOrEqual:
            return "GreaterThanOrEqual";
        case Tag::LessThan:
            return "LessThan";
        case Tag::LessThanOrEqual:
            return "LessThanOrEqual";
        case Tag::And:
            return "And";
        case Tag::Or:
            return "Or";
        case Tag::Not:
            return "Not";
        case Tag::Var:
            return "Var";
        case Tag::Def:
            return "Def";
        case Tag::Assign:
            return "Assign";
        case Tag::LeftBrace:
            return "LeftBrace";
        case Tag::RightBrace:
            return "RightBrace";
        case Tag::LeftBracket:
            return "LeftBracket";
        case Tag::RightBracket:
            return "RightBracket";
        case Tag::LeftParenthesis:
            return "LeftParenthesis";
        case Tag::RightParenthesis:
            return "RightParenthesis";
        case Tag::Dot:
            return "Dot";
        case Tag::Comma:
            return "Comma";
        case Tag::Semicolon:
            return "Semicolon";
        case Tag::Colon:
            return "Colon";
        case Tag::RightArrow:
            return "RightArrow";
        case Tag::True:
            return "True";
        case Tag::False:
            return "False";
        case Tag::If:
            return "If";
        case Tag::Else:
            return "Else";
        case Tag::While:
            return "While";
        case Tag::For:
            return "For";
        case Tag::Break:
            return "Break";
        case Tag::Return:
            return "Return";
        case Tag::Import:
            return "Import";
        case Tag::Uses:
            return "Uses";
        case Tag::LowerBound:
            return "LowerBound";
        case Tag::UpperBound:
            return "UpperBound";
        case Tag::Class:
            return "Class";
        case Tag::Private:
            return "Private";
        case Tag::Protected:
            return "Protected";
        case Tag::Module:
            return "Module";
        case Tag::Eof:
            return "Eof";
        default:
            std::cout << __FUNCTION__ << std::endl;
            exit(1);
    }
}
