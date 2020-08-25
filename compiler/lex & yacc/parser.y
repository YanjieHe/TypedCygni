%{
#include "Expression.hpp"
#include <iostream>
#include <stdio.h>
void yyerror(const char *s);
int yylex();
Position Pos();
extern int yylineno;
extern int yycolno;
Expression* parsedTree;
string currentInputSourcePath;
vector<SyntaxError> syntaxErrorList;
%}

%union {
    Expression* expr;
    Statement* stmt;
    Statement* stmtList;
    BlockStatement* block;
}

%code requires {
#include "Expression.hpp"
}

%token TOKEN_ID
%token <expr> TOKEN_INTEGER
%token <expr> TOKEN_FLOAT
%token TOKEN_GE
%token TOKEN_LE
%token TOKEN_EQ
%token TOKEN_NE
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_NOT
%token TOKEN_VAR
%token TOKEN_DEF
%token <expr> TOKEN_TRUE
%token <expr> TOKEN_FALSE
%token TOKEN_IF
%token TOKEN_ELSE
%token TOKEN_WHILE
%token TOKEN_RETURN
%token TOKEN_CLASS
%token TOKEN_MODULE
%token TOKEN_IMPORT
%token TOKEN_NEW
%token <expr> TOKEN_STRING
%token TOKEN_UNEXPECTED

%type <expr> Expr
%type <expr> Term
%type <expr> Factor

%type <stmtList> StatementList
%type <block> Block

%start Program

%%
Program
    : Expr { parsedTree = $1; }
    ;

MethodDecl
    : TOKEN_DEF TOKEN_ID '(' ')' ':' TOKEN_ID Block
    ;

Block
    : '{' '}' { $$ = new BlockStatement(Pos(), vector<Statement *>{}); }
    | '{' StatementList '}' {
        $$ = new BlockStatement(Pos(), Vec::SinglyLinkedListToVector<StatementList *, Statement*>(
            $1
        ));
    }
    ;

StatementList
    : Statement {
        $$ = new StatementList($1, NULL);
    }
    | StatementList Statement {
        $$ = new StatementList($2, $1);
    }
    ;

Statement
    : ExpressionStatement
    ;

ExpressionStatement
    : Expr
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
    : TOKEN_INTEGER { $$ = $1; }
    | TOKEN_FLOAT { $$ = $1; }
    | TOKEN_TRUE { $$ = $1; }
    | TOKEN_FALSE { $$ = $1; }
    | TOKEN_STRING { $$ = $1; }
    ;
%%

void yyerror(const char* s) {
	fflush(stdout);
    syntaxErrorList.push_back(
        SyntaxError(currentInputSourcePath, Pos(), s)
    );
	printf("\n%*s\n%*s\n", yycolno, "^", yycolno, s);
}

Position Pos() {
    return Position(yylineno - 1, yycolno);
}