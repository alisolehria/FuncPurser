%{
#include "tokens.h"
%}

DIGIT    [0-9]
IDENT	[a-zA-Z][A-Za-z0-9]*
         
%%

";" 		{ return SEMI;}
":="	 	{ return ASSIGN;}
","	 	{ return COMMA;}
"function"	{ return FUNCTION;}
"returns"	{ return RETURNS;}
"if"		{ return IF;}
"end if"	{ return ENDIF;}
"then"		{ return THEN;}
"else"		{ return ELSE;}
"begin"		{ return TBEGIN;}
"end loop" 	{ return ENDLOOP;}
"end function" 	{ return ENDFUNCTION;}
"while"		{ return WHILE;}
"loop"	 	{ return LOOP;}
"Plus"  	{ return PLUS;}
"Minus"		{ return MINUS;}  
"read"		{ return READ;}  
"write"		{ return WRITE;} 
"variables" 	{ return VARIABLES;}
"Times" 	{ return TIMES;}
"Divide" 	{ return DIVIDE;}
"(" 		{ return LBRA;}
")" 		{ return RBRA;}
"Less" 		{ return LT;}
"LessEq" 	{ return LTE;}
"Eq" 		{ return EQ;}
"NEq" 		{ return NEQ;}
"Main"		{ return MAIN;}
{DIGIT}+ 	{ return INT;}
{IDENT}	 	{ return ID;}
<<EOF>>	 	{ return EOF;}

[ \t\n]+          /* eat up whitespace */


%%

int yywrap() { return EOF; }
