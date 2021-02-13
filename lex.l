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

DIGIT	[0-9]
LETTER	[a-zA-Z]
id	{letter}|_({letter}|{digit})*
digits	{digit}+
opFraction	(\.{digits})?
opExponent	([Ee][+-]?{digits})?
number	{digits}{opFraction}{opExponent}

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




