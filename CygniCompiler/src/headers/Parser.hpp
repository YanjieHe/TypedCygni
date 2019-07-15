#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "Token.hpp"
#include "Encoding.hpp"
#include "Ast.hpp"
#include "Type.hpp"
#include <iostream>
#include "Predef.hpp"
#include "Exception.hpp"

using std::cout;
using std::endl;

class Parser
{
private:
    Vector<Token> tokens;
    String path;
    int offset;
public:
    Parser(String path, Vector<Token> tokens);

    Program ParseProgram();

    inline bool IsEof() const
    {
        return Look().tag == Tag::Eof;
    }

    inline const Token &Look() const
    {
        return tokens[offset];
    }

    inline void Move()
    {
        offset++;
    }

    Token Match(Tag tag)
    {
        if (tag == Look().tag)
        {
            Token t = Look();
            Move();
            return t;
        }
        else
        {
            String message =
                    String("expecting '") + TagToString(tag) + String("', got '") + TagToString(Look().tag) + "'";
            throw ParserException(Look().line, Look().column, message);
        }
    }

    inline Position GetPos(const Token &token) const
    {
        return {token.line, token.column, Look().line, Look().column};
    }


    Ptr<Ast> Statement();

    Ptr<Ast> ParseAssign();

    Ptr<Ast> ParseOr();

    Ptr<Ast> ParseAnd();

    Ptr<Ast> ParseEquality();

    Ptr<Ast> ParseRelation();

    Ptr<Ast> ParseExpr();

    Ptr<Ast> ParseTerm();

    Ptr<Ast> ParseUnary();

    Ptr<Ast> ParsePostfix();

    Ptr<Ast> ParseFactor();

    Ptr<Ast> ParseBlock();

    Ptr<Ast> IfStatement();

    Ptr<Var> ParseVar();

    Ptr<Var> ParseVarDeclaration();

    Ptr<Def> ParseDef();

    Parameter ParseParameter();

    Ptr<TypeExpression> ParseType();

    Ptr<Ast> ParseReturn();

    Vector<Ptr<TypeExpression>> ParseTypeArguments();

    Ptr<While> ParseWhile();

    Ptr<DefClass> ParseDefClass();

    Ptr<DefModule> ParseDefModule();

    Access ParseAccess();
};


#endif // PARSER_HPP