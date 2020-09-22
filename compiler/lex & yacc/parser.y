%{
#include "Expression.hpp"
#include <iostream>
#include <stdio.h>
using std::cout;
using std::endl;

class Expression;

void yyerror(const char *s);
int yylex();
Position Pos();

extern int yylineno;
extern int yycolno;

Statement* parsedTree;
string currentInputSourcePath;
vector<SyntaxError> syntaxErrorList;
shared_ptr<ExpressionManager> ExpManager;
shared_ptr<TokenCreator> tokenCreator;
%}

%union {
    Expression* expr;
    Statement* stmt;
    SLinkedList<Statement *> *stmtList;
    BlockStatement* block;
    MethodDeclStatement *methodDecl;
    Token *token;
}

%code requires {
#include "Expression.hpp"
}

%code top {
#include "Expression.hpp"
}

%token <token> TOKEN_ID
%token <token> TOKEN_INTEGER
%token <token> TOKEN_FLOAT
%token TOKEN_GE
%token TOKEN_LE
%token TOKEN_EQ
%token TOKEN_NE
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_NOT
%token TOKEN_VAR
%token TOKEN_DEF
%token <token> TOKEN_TRUE
%token <token> TOKEN_FALSE
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_WHILE
%token TOKEN_RETURN
%token TOKEN_CLASS
%token TOKEN_MODULE
%token TOKEN_IMPORT
%token TOKEN_NEW
%token <token> TOKEN_STRING
%token TOKEN_UNEXPECTED

%type <expr> Expr
%type <expr> Term
%type <expr> Factor

%type <stmtList> StatementList
%type <block> Block
%type <methodDecl> MethodDecl
%type <stmt> Statement
%type <expr> ExpressionStatement
%type <stmt> VarDeclStatement

%start Program

%%
Program
    : MethodDecl { cout << "program" << endl; parsedTree = $1; }
    ;

Modules
    : ModuleDecl { cout << "Program" << endl; }
    ;

MethodDecl
    : TOKEN_DEF TOKEN_ID '(' ')' ':' TOKEN_ID Block {
        cout << "METHOD DECL" << endl;
        $$ = new MethodDeclStatement(
            Pos(), $2->text, vector<Parameter *>(), $7
        );
    }
    ;

Block
    : '{' '}' { $$ = new BlockStatement(Pos(), vector<Statement *>{}); }
    | '{' StatementList '}' {
        $$ = new BlockStatement(Pos(), Vec::SLinkedListToVec<Statement *>(
            $2
        ));
    }
    ;

StatementList
    : Statement {
        $$ = new SLinkedList<Statement *>($1, nullptr);
    }
    | StatementList Statement {
        $$ = new SLinkedList<Statement *>($2, $1);
    }
    ;

Statement
    : ExpressionStatement { $$ = $1; }
    | VarDeclStatement { $$ = $1; }
    ;

VarDeclStatement
    : TOKEN_VAR TOKEN_ID ':' TOKEN_ID {
        $$ = new VarDeclStatement(Pos(), $2->text,
            optional<TypePtr>(), optional<Expression *>());
    }
    ;

ExpressionStatement
    : Expr { $$ = $1; }
    ;

Expr
    : Term
    | Expr '+' Term {
        $$ = new BinaryExpression(Pos(), ExpressionType::ADD, $1, $3);
    }
    | Expr '-' Term {
        $$ = new BinaryExpression(Pos(), ExpressionType::SUBTRACT, $1, $3);
    }
    ;

Term
    : Factor
    | Term '*' Factor {
        $$ = new BinaryExpression(Pos(), ExpressionType::MULTIPLY, $1, $3);
    }
    | Term '/' Factor {
        $$ = new BinaryExpression(Pos(), ExpressionType::DIVIDE, $1, $3);
    }
    | Term '%' Factor {
        $$ = new BinaryExpression(Pos(), ExpressionType::MODULO, $1, $3);
    }
    ;

Factor
    : TOKEN_INTEGER { $$ = new ConstantExpression(Pos(), ExpressionType::INT, $1->text); }
    | TOKEN_FLOAT { $$ = new ConstantExpression(Pos(), ExpressionType::FLOAT, $1->text); }
    | TOKEN_TRUE { $$ = new ConstantExpression(Pos(), ExpressionType::BOOLEAN, $1->text); }
    | TOKEN_FALSE { $$ = new ConstantExpression(Pos(), ExpressionType::BOOLEAN, $1->text); }
    | TOKEN_STRING { $$ = new ConstantExpression(Pos(), ExpressionType::STRING, $1->text); }
    ;
%%

void yyerror(const char* s) {
    syntaxErrorList.push_back(
        SyntaxError(currentInputSourcePath, Pos(), s)
    );
}

Position Pos() {
    return Position(yylineno - 1, yycolno);
}