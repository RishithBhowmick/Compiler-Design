%{
#include<stdio.h>
void yyerror(char *s);
//#include "y.tab.h" token declarations from yacc file
int linenumber=0;
%}

%x IN_MULTILINE_COMMENT
%x IN_SINGLELINE_COMMENT

/*
A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]
int	return INT;
float	return FLOAT;
{id}	return ID;
{number}	return NUM;
[\n|\t|' ']	;
.	return *yytext;
 */

digit	[0-9]
letter	[a-zA-Z]
id	{letter}|_({letter}|{digit})*
digits	{digit}+
opFraction	(\.{digits})?
opBool	true|false
opString	\".*\"
opExponent	([Ee][+-]?{digits})?
number	{digits}{opFraction}{opExponent}
// single eq is used as assignment operator only in const and var blocks.
// Otherwise it is used as the relational operator ==
// single eq also used to define variable type in the type block
// type block is used for classes (need to use class keyword for this) or enumerated types
single_eq	=
single_char_op	<|>|\+|\*|-|\/|\%|\&|\||\!|\~
multiple_char_op	and|or|not|div|mod|and\ then|or\ else|<<|>>|<>|>=|<=|:=|\+=|\*=|-=|\/=|\%=
paranthesis	\(|\)

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

and				return T_AND;
or				return T_OR;
array			return T_ARRAY;
const 			return T_CONST;
if				return T_IF;
then 			return T_THEN;
else			return T_ELSE:
for				return T_FOR;
to				return T_TO;
downto			return T_DOWNTO;
do				return T_DO;
read			;
readln			;
write			return T_WRITE;
writeln			return T_WRITELN;
[\t|' ']+		;

{id} {
	if(yyleng > 31) {
		printf("Warning : Identifier Length Greater than 31 characters, Truncating Identifier.\n");
	}
	char temp[32];
	strncpy(temp,yytext,31);
	yylval.str = strdup(temp);
	ECHO;
 return T_IDENTIFIER;
}
