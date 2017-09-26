%{
#include "bf.tab.h"
%}
%option noyywrap
%x FILESTRING

%%

\n+					{ yylval.length=yyleng; return NEWLINES;}
[[:blank:]]			;
\++					{ yylval.length=yyleng; return PLUS; }
-+					{ yylval.length=yyleng; return MINUS; }
>+					{ yylval.length=yyleng; return RIGHT; }
\<+					{ yylval.length=yyleng; return LEFT; }
[[\].,:;^]			{ return yytext[0]; }
	/* Filenames containing '%' will not be supported for now */
%					{ BEGIN(FILESTRING); }
<FILESTRING>[^%]+	{ yylval.string=strdup(yytext); return FILENAME; }
<FILESTRING>%		{ BEGIN(INITIAL); }
