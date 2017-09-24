%{
#include "bf.tab.h"
%}
%option noyywrap

%%

\n+					{ yylval.length=yyleng; return NEWLINES;}
[[:blank:]]			;
\++					{ yylval.length=yyleng; return PLUS; }
-+					{ yylval.length=yyleng; return MINUS; }
>+					{ yylval.length=yyleng; return RIGHT; }
\<+					{ yylval.length=yyleng; return LEFT; }
[[\].,]				{ return yytext[0]; }
