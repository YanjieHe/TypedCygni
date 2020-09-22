%{
#include "Parser.hpp"
#include <iostream>
using std::cout;
using std::endl;
extern int yylineno;
int yycolno = 0;
void count();
extern Position Pos();
extern shared_ptr<TokenCreator> tokenCreator;
%}

DIGIT [0-9]
SIGN [+-]
ALPHA [A-Za-z]
ALNUM [A-Za-z0-9]

ASC     [\x00-\x7f]
ASCN    [\x00-\t\v-\x7f]
U       [\x80-\xbf]
U2      [\xc2-\xdf]
U3      [\xe0-\xef]
U4      [\xf0-\xf4]

UANY    {ASC}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}
UANYN   {ASCN}|{U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}
UONLY   {U2}{U}|{U3}{U}{U}|{U4}{U}{U}{U}

%%
{DIGIT}+(\.{DIGIT}+)([eE]{SIGN}?{DIGIT}+) {
	count();
	yylval.token = new Token(yylineno - 1, yycolno, Tag::DOUBLE, yytext);
	return TOKEN_FLOAT;
}
{DIGIT}+([eE]{SIGN}?{DIGIT}+) {
	count();
	yylval.token = new Token(yylineno - 1, yycolno, Tag::DOUBLE, yytext);
	return TOKEN_FLOAT;
}
{DIGIT}+(\.{DIGIT}+) {
	count();
	yylval.token = new Token(yylineno - 1, yycolno, Tag::DOUBLE, yytext);
	return TOKEN_FLOAT;
}
{DIGIT}+ {
	count();
	yylval.token = new Token(yylineno - 1, yycolno, Tag::INTEGER, yytext);
	return TOKEN_INTEGER;
}

"+"|"-"|"*"|"/"|"%" { count(); return yytext[0]; }
">=" { count(); return TOKEN_GE; }
"<=" { count(); return TOKEN_LE; }
"==" { count(); return TOKEN_EQ; }
"!=" { count(); return TOKEN_NE; }
">"|"<"|"="|"."|","|":" { count(); return yytext[0]; }
"("|")"|"["|"]"|"{"|"}" { count(); return yytext[0]; }
"and" { count(); return TOKEN_AND; }
"or" { count(); return TOKEN_OR; }
"not" { count(); return TOKEN_NOT; }
"var" { count(); return TOKEN_VAR; }
"def" { count(); return TOKEN_DEF; }
"true" {
	count();
	yylval.token = new Token(yylineno - 1, yycolno, Tag::BOOLEAN_T, yytext);
	return TOKEN_TRUE;
}
"false" {
	count();
	yylval.token = new Token(yylineno - 1, yycolno, Tag::BOOLEAN_F, yytext);
	return TOKEN_FALSE;
}
"if" { count(); return TOKEN_IF; }
"else" { count(); return TOKEN_ELSE; }
"while" { count(); return TOKEN_WHILE; }
"return" { count(); return TOKEN_RETURN; }
"class" { count(); return TOKEN_CLASS; }
"module" { count(); return TOKEN_MODULE; }
"import" { count(); return TOKEN_IMPORT; }
"new" { count(); return TOKEN_NEW; }
("_"|{ALPHA})("_"|{ALNUM})* {
	count();
	yylval.token = new Token(yylineno - 1, yycolno, Tag::ID, yytext);
	return TOKEN_ID;
}
\"({UANYN}|[^\\"])*\" {
	count();
	yylval.token = new Token(yylineno - 1, yycolno, Tag::STRING, yytext);
	return TOKEN_STRING;
}
"//"{UANYN}* { count(); /* skip comments */ }
(" "|"\t"|"\v"|"\r"|"\n"|"\f")+ { count(); /* skip whitespaces */ }
. { return TOKEN_UNEXPECTED; }
%%

int yywrap() {
    return(1);
}

void count() {
	int i;

	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n') {
			yycolno = 0;
        } else {
			yycolno++;
        }
    }
}
