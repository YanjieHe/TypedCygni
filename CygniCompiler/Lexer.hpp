#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include "Token.hpp"
#include "Encoding.hpp"
#include "Exception.hpp"

class Lexer
{
private:
    String code;
    int line;
    int column;
    int offset;
    String builder;
    static std::unordered_map<String, Tag> operators;

public:
    explicit Lexer(String code);

    std::vector<Token> ReadAll();

private:

    Token ReadInt();

    Token ReadFloat();

    Token ReadString();

    Char UnescapedChar(Char c);

    Token ReadIdentifier();

    Token ReadOperator();

    void SkipWhitespaces();

    inline static bool IsIdentifierChar(Char c)
    {
        return IsLetter(c) || IsDigit(c) || c == '_';
    }

    inline bool IsEof()
    {
        return offset >= static_cast<int32_t>(code.size());
    }

    void Forward()
    {
        if (code.at(offset) == '\n')
        {
            offset++;
            line++;
            column = 1;
        }
        else
        {
            offset++;
            column++;
        }
    }

    inline Char Peek()
    {
        return code.at(offset);
    }

    void Consume()
    {
        builder.push_back(Peek());
        Forward();
    }

    void Reset()
    {
        builder.clear();
    }

    inline static bool IsDigit(Char c)
    {
        return (c >= '0' && c <= '9');
    }

    inline static bool IsLetter(Char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    inline static bool IsWhiteSpace(Char c)
    {
        return c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\n';
    }
};


#endif // LEXER_HPP