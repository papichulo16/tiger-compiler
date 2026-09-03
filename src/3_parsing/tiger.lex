%{
#include <string.h>
#include "util.h"
#include "y.tab.h"
#include "errormsg.h"

int charPos=1;

int yywrap(void)
{
 charPos=1;
 return 1;
}


void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}

%}

DIGITS [0-9]+
IDENT [a-zA-Z][a-zA-Z0-9]*
STR ["][^"]*["]
COMMENT [/][*]([^*]|[*][^/])*[*][/]

%%
" "	 {adjust(); continue;}
\n	 {adjust(); EM_newline(); continue;}

{COMMENT} {adjust(); continue;}

":=" {adjust(); return ASSIGN;}

","	 {adjust(); return COMMA;}
":"	 {adjust(); return COLON;}
";"	 {adjust(); return SEMICOLON;}
"("	 {adjust(); return LPAREN;}
")"	 {adjust(); return RPAREN;}
"["	 {adjust(); return LBRACK;}
"]"	 {adjust(); return RBRACK;}
"{"	 {adjust(); return LBRACE;}
"}"	 {adjust(); return RBRACE;}

"." {adjust(); return DOT;}
"+" {adjust(); return PLUS;}
"-" {adjust(); return MINUS;}
"*" {adjust(); return TIMES;}
"/" {adjust(); return DIVIDE;}

"=" {adjust(); return EQ;}
"!=" {adjust(); return NEQ;}
"<=" {adjust(); return LE;}
">=" {adjust(); return GE;}
"<" {adjust(); return LT;}
">" {adjust(); return GT;}

"&" {adjust(); return AND;}
"|" {adjust(); return OR;}

let {adjust(); return LET;}
type {adjust(); return TYPE;}
var {adjust(); return VAR;}
while {adjust(); return WHILE;}
for {adjust(); return FOR;}
nil {adjust(); return NIL;}
to {adjust(); return TO;}
break {adjust(); return BREAK;}
in {adjust(); return IN;}
end {adjust(); return END;}
function {adjust(); return FUNCTION;}
array {adjust(); return ARRAY;}
if {adjust(); return IF;}
then {adjust(); return THEN;}
else {adjust(); return ELSE;}
do {adjust(); return DO;}
of {adjust(); return OF;}

{STR} {adjust(); yylval.sval=strdup(yytext); return STRING;}
{DIGITS}	 {adjust(); yylval.ival=atoi(yytext); return INT;}
{IDENT} {adjust(); yylval.sval = strdup(yytext); return ID;}

.	 {adjust(); EM_error(EM_tokPos,"illegal token");}

