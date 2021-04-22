%option noyywrap yylineno

%{
#include<bits/stdc++.h>
#include"syntax_tree.h"
#include"tiny.tab.h"
using namespace std;
%}

Integer ([1-9][0-9]*|[0])

%%

"IF" {return IF;}
"ELSE" {return ELSE;}
"FOR" {return FOR;}
"WRITE" {return WRITE;}
"READ" {return READ;}
"RETURN" {return RETURN;}
"BEGIN" {return tkBEGIN;}
"END" {return END;}
"MAIN" {return MAIN;}
"INT" {return INT;}
"REAL" {return REAL;}
"CHAR" {return CHAR;}

"+" |
"-" |
"*" |
"/" |
"<" |
"(" |
")" |
"," |
";" {return yytext[0];}
":=" {return Assign;}
"==" {return Equal;}
"!=" {return NEqual;}
"<=" {return LE;}

{Integer} {yylval.Number_Integer=atoi(yytext); return Number_Integer;}
{Integer}"."[0-9]+ {yylval.Number_Float=atof(yytext); return Number_Float;}
[a-zA-Z_][a-zA-Z_0-9]* {yylval.IdString=yytext; return IdString;}
\"..*\" {yylval.QString=yytext; return QString;}

[ \t\n\r]+
"/**".*"**/"

. {printf("%d: Undefined token\n",yylineno);}

%%