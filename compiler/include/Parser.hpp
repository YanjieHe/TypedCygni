/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 33 "parser.y"

#include "Expression.hpp"

#line 52 "parser.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_ID = 258,
    TOKEN_INTEGER = 259,
    TOKEN_FLOAT = 260,
    TOKEN_GE = 261,
    TOKEN_LE = 262,
    TOKEN_EQ = 263,
    TOKEN_NE = 264,
    TOKEN_AND = 265,
    TOKEN_OR = 266,
    TOKEN_NOT = 267,
    TOKEN_VAR = 268,
    TOKEN_DEF = 269,
    TOKEN_TRUE = 270,
    TOKEN_FALSE = 271,
    TOKEN_IF = 272,
    TOKEN_ELSE = 273,
    TOKEN_WHILE = 274,
    TOKEN_RETURN = 275,
    TOKEN_CLASS = 276,
    TOKEN_MODULE = 277,
    TOKEN_IMPORT = 278,
    TOKEN_NEW = 279,
    TOKEN_STRING = 280,
    TOKEN_UNEXPECTED = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "parser.y"

    Expression* expr;
    Statement* stmt;
    SLinkedList<Statement *> *stmtList;
    BlockStatement* block;
    MethodDeclStatement *methodDecl;
    Token *token;

#line 99 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
