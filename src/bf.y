%{

%}

%%

code:
		code exp
	|	exp
	;
exp:
		'[' exp ']'
	|	exp op
	|	op
	;
op:
		'+'
	|	'-'
	|	'<'
	|	'>'
	|	'.'
	|	','
	;

%%

