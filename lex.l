%{
	#include<stdio.h>
	#include<string.h>
	void yyerror(char *s);
	#include "y.tab.h" //token declarations from yacc file
	#include "uthash/src/uthash.h"
	int yycolumn;
	// #define YY_USER_ACTION yyloc.first_line = yyloc.last_line = yylineno;\
	// yyloc.first_column = yycolumn; yyloc.last_column = yycolumn + yyleng - 1;\
	// yycolumn += yyleng;
	int linenumber = 0;
%}
%option yylineno

%x IN_MULTILINE_COMMENT
%x IN_SINGLELINE_COMMENT

digit	[0-9]
letter	[a-zA-Z]
id	{letter}|_({letter}|{digit})*
digits	{digit}+
opFraction	(\.{digits})?
opBool	true|false
opString	\".*\"
opExponent	([Ee][+-]?{digits})?
number	{digits}{opFraction}{opExponent}
single_eq	=
single_char_op	<|>|\+|\*|-|\/|\%|\&|\||\!|\~
multiple_char_op	and|or|not|<<|>>|<>|>=|<=|:=|\+=|\*=|-=|\/=
paranthesis	\(|\)|\[|\]
punctuaton ;|,|:|.
indextype [0-9]\.\.\.[0-9]+
intval digits
floatval number
boolval true|false
stringval \".*\"
datatypes integer|character|real|boolean|string

%%

<INITIAL>"(*"	{BEGIN(IN_MULTILINE_COMMENT);}
")*"			{printf("Unmatching comment marks {\n");
				yyterminate();}				
<INITIAL>"{"	{BEGIN(IN_SINGLELINE_COMMENT);}
"}"			{printf("Unmatching comment marks {*\n");
				yyterminate();}					
<IN_MULTILINE_COMMENT>"*)"	{BEGIN(INITIAL);}
<IN_MULTILINE_COMMENT>.	;
<IN_MULTILINE_COMMENT>[^*\n]*	;
<IN_MULTILINE_COMMENT>"*"+[^*/\n]*	;
<IN_MULTILINE_COMMENT>\n	linenumber++;
<IN_SINGLELINE_COMMENT>"}"	{BEGIN(INITIAL);}
<IN_SINGLELINE_COMMENT>.	;


program			return T_PROGRAM;
uses			return T_USES;
type			return T_TYPE;
var				return T_VAR;
begin 			return T_BEGIN;
end 			return T_END;
function		return T_FUNCTION;
procedure		return T_PROCEDURE;

{indextype}		return T_INDEXTYPE;
{intval}		{
	yylval.s.intval = atoi(yytext);
	ECHO;
	return T_INTVAL;
}
{floatval} 		{
	yylval.s.floatval = atoi(yytext);
	ECHO;
	return T_FLOATVAL;
}
{boolval} 		{
	if(strcmp(yytext, "true")==0) 
		yylval.s.intval = 1;
	else
		yylval.s.intval = 0;
	ECHO;
	return T_BOOLVAL;
}

array			return T_ARRAY;
of 				return T_OF;
const 			return T_CONST;
if				return T_IF;
then 			return T_THEN;
else			return T_ELSE;
for				return T_FOR;
to				return T_TO;
downto			return T_DOWNTO;
do				return T_DO;
read			;
readln			;
write			return T_WRITE;
writeln			return T_WRITELN;
[\t|' ']+		;

{datatypes} 	{
	yylval.s.type = strdup(yytext);
	ECHO;
	return T_DATATYPE;
}

{id} {
	if(yyleng > 31) {
		printf("Warning : Identifier Length Greater than 31 characters, Truncating Identifier.\n");
	}
	char temp[32];
	strncpy(temp,yytext,31);
	yylval.s.str = strdup(temp);
	ECHO;
	return T_IDENTIFIER;
}

{single_char_op}   {
	return yytext[0];
}

{single_eq}   {
	return T_SINGLEEQ;
}

{multiple_char_op}  {
	if(strcmp(yytext, "and") == 0) {
		return T_BOOL_AND;
	}
	else if(strcmp(yytext, "or") == 0) {
		return T_BOOL_OR;
	}
	else if(strcmp(yytext, "not") == 0) {
		return T_BOOL_NOT;
	}
	else if(strcmp(yytext, "<=") == 0) {
		return T_LE;
	}
	else if(strcmp(yytext, ">=") == 0) {
		return T_GE;
	}
	else if(strcmp(yytext, "<>") == 0) {
		return T_NE;
	}
	else if(strcmp(yytext, "<<") == 0) {
		return T_BIT_LEFT_SHIFT;
	}
	else if(strcmp(yytext, ">>") == 0) {
		return T_BIT_RIGHT_SHIFT;
	}
	else if(strcmp(yytext, ":=") == 0) {
		return T_ASOP;
	}
	else if(strcmp(yytext, "+=") == 0) {
		return T_AS_PE;
	}
	else if(strcmp(yytext, "-=") == 0) {
		return T_AS_SE;
	}
	else if(strcmp(yytext, "*=") == 0) {
		return T_AS_MULE;
	}
	else if(strcmp(yytext, "/=") == 0) {
		return T_AS_DIVE;
	}
}

{paranthesis} {
	return yytext[0];
}

{punctuaton} {
	return yytext[0];
}

"\n" {
	yycolumn = 1;
}

%%

int yywrap() {
	return 1;
}
