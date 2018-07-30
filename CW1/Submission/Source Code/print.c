#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

extern int symb;
extern char * yytext;

char * showSymb(int symb)
{  switch(symb)
   {  case  TBEGIN: return "begin";
      case  LOOP: return "loop";
      case  FUNCTION: return "function";
      case  RETURNS: return "returns";
      case  ENDFUNCTION: return "end function";
      case  ENDLOOP: return "end loop";
      case  ELSE: return "else";
      case  ENDIF: return "end if";
      case  ID: return "ID";
      case  IF: return "if";
      case  INT: return "INT";
      case  READ: return "read";
      case  WRITE: return "write";
      case  THEN: return "then";
      case  VARIABLES: return "variables";
      case  WHILE: return "while";         
      case  PLUS: return "Plus";
      case  MINUS: return "Minus";
      case  TIMES: return "Times";
      case  DIVIDE: return "Divide";
      case  LBRA: return "(";
      case  RBRA: return ")";
      case  LT: return "Less";
      case  LTE: return "LessEq";
      case  EQ: return "Eq";
      case  NEQ: return "NEq";
      case  ASSIGN: return ":=";
      case SEMI: return ";";
      case COMMA: return ",";
      case MAIN: return "Main";
      case EOF: return "EOF";
      default: printf("bad symbol: %d",symb);
   }
}

printSymb()
{  char * s;
   printf("%s ",showSymb(symb));
   if(symb==ID || symb==INT){
    if(yytext == NULL)
      printf("Error: yytext is null");
    else
      printf("%s\n",yytext);
   } else
    printf("\n");
}
