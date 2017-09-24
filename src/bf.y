%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern FILE *yyin;

void yyerror(const char* s);
%}

%union {
	int length;	// Holds the number of matched operations
	char* string;
}
// Tokens for matching strings of the same operation
%token <length> PLUS
%token <length> MINUS
%token <length> RIGHT
%token <length> LEFT
%token <length> NEWLINES

%error-verbose
%%

file:
		expressions
	;
expressions:
		expressions expression
	|	expression
	;
expression:
		operations
	|	'[' { puts("Entering loop"); } expressions ']' { puts("Exiting loop"); }
	;
operations:
		operations operation
	|	operation
	;
operation:
		PLUS	{ printf("%d +\n", yylval.length); }
	|	MINUS	{ printf("%d -\n", yylval.length); }
	|	LEFT	{ printf("%d <\n", yylval.length); }
	|	RIGHT	{ printf("%d >\n", yylval.length); }
	|	'.'		{ puts("."); }
	|	','		{ puts(","); }
	|	NEWLINES	{ printf("%d newline(s)\n", yylval.length); }
	;

%%

int main(int argc, char **argv)
{
	if(argc>1) {
		FILE* tmp = fopen(argv[1], "r");
		if(tmp != NULL)
			yyin = tmp;
	}
	do {
		yyparse();
	} while (!feof(yyin));
	return 0;
}

void yyerror(const char *s) {
	printf("Parse error: %s\n", s);
	exit(1);
}
