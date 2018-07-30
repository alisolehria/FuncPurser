#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
#include "nodes.h"

int symb;
extern printSymb();
extern char* showSymb(int);
extern int yylex(void);
extern FILE* yyin;

extern void prettytree(NODE*,int);
extern char * yytext;

lex()
{
	//printSymb();
	symb = yylex();
}

NODE * newInt(int v)
{  NODE * n;
   n = (NODE *)malloc(sizeof(NODE));
   n->tag = INT;
   n->f.value = v;
   return n;
}


NODE * newId(char * i)
{  NODE * n;
   char * cur = strdup(i);
   n = (NODE *)malloc(sizeof(NODE));
   n->tag = ID;
   n->f.id = cur;
   return n;
}

NODE * newNode(int tag)
{  NODE * n;
   n = (NODE *)malloc(sizeof(NODE));
   n->tag = tag;
   n->f.b.n1 = NULL;
   n->f.b.n2 = NULL;
   return n;
}

showTree(NODE * tree,int depth)
{  int i;
   if(tree==NULL)
    return;
   for(i=0;i<depth;i++)
    putchar('-');
   switch(tree->tag)
   {  case ID: printf("%s\n",tree->f.id);
               return;
      case INT: printf("%d\n",tree->f.value);
               return;
      default: printf("%s\n",showSymb(tree->tag));
               showTree(tree->f.b.n1,depth+1);
               showTree(tree->f.b.n2,depth+1);
   }
}

error(char * rule,char * message)
{  printf("%s: found %s\n",rule,showSymb(symb));
   printf("%s: %s\n",rule,message);
   exit(0);
}

NODE *program()
{	
	extern NODE * functions();
	NODE * p;
	p = newNode(SEMI);
	if(symb==FUNCTION){
		p->f.b.n2=functions();
	}
	return p;


}

NODE *functions()
{
	extern NODE * function();
	NODE * c;
	c = function();
	if(symb==SEMI)
	{
		lex();
		if(symb==FUNCTION){
			NODE * c1;
         	c1 = c;
         	c = newNode(SEMI);
         	c->f.b.n1 = c1;
         	c->f.b.n2 = functions();
		}
	}
	else{
		error("functions","; expected");
	}
	return c;	
}

NODE * function()
{
   extern NODE * args();
   extern NODE * commands();
   NODE * fun;
   NODE * func;
   NODE * a;
   NODE * r;
   NODE * v;

   
   if(symb!=FUNCTION)
      error("function","function expected");
   lex();
   if(symb!=ID && symb!=MAIN)
      error("function","name expected");
   fun=newNode(FUNCTION);
   func = newNode(SEMI);
   fun->f.b.n1= newId(yytext);
   fun ->f.b.n2=func;
   if(symb==MAIN){
      lex();
      if(symb!=LBRA)
         error("function","( expected");
      lex();
      a = newNode(LBRA);
      if(symb!=RBRA)
         error("function",") expected");
      lex();
      r = newNode(RETURNS);
      a->f.b.n1=r;
   }
   else{
      lex();
      if(symb!=LBRA)
         error("function","( expected");
      lex();
      a = newNode(LBRA);
      if(symb==ID){
         
         a->f.b.n1=args();
      }
      if(symb!=RBRA)
         error("function",") expected");
      lex();
      r = newNode(RETURNS);
      if(symb==RETURNS){
         
         lex();
         r->f.b.n1= newId(yytext);
         a->f.b.n2=r;
         if(symb!=ID)
            error("function","name expected");
      lex();
      }
   }
   func->f.b.n1=a;

   if(symb==VARIABLES){
      lex();
      v = newNode(VARIABLES);
      v->f.b.n1=args();
      r->f.b.n2=v;
   }
  
   if(symb!=TBEGIN)
      error("function","begin expected");
   lex();
   func->f.b.n2=commands();
   if(symb!=ENDFUNCTION)
      error("function","end function expected");
   lex();
   return fun;
}

NODE * args(){
   NODE * a;

   if(symb!=ID)
      error("args","name expected");
   a = newNode(COMMA);
   a->f.b.n1=newId(yytext);
   lex();

   if(symb==COMMA){
      a->f.b.n2= newNode(VARIABLES);
      lex();
      a->f.b.n2->f.b.n1=args();
   }
   return a;
}

NODE * commands(){
   extern NODE * command();
   NODE *c;
   c = command();
   if(symb!=SEMI)
      error("commands","; expected");
   lex();
   if(symb==ID||symb==IF||symb==WHILE||symb==READ||symb==WRITE){
      NODE *c1;
      c1=c;
      c = newNode(SEMI);
      c->f.b.n1=c1;
      c->f.b.n2=commands();
   }
   return c;
}

NODE * command(){
   extern NODE * assign();
   extern NODE * ifComm();
   extern NODE * whileComm();
   extern NODE * read();
   extern NODE * write();

   switch(symb)
   {
      case ID:    return assign();
               
      case IF:    lex();
                  return ifComm();
             
      case WHILE: lex();
                  return whileComm();
             
      case READ:  lex();
                  return read();
               
      case WRITE: lex();
                  return write();
               

      default: error("command","name/if/while/read/write expected");


   }

}

NODE * assign(){
   extern NODE * expr();

   NODE *a;
   
   a = newNode(ASSIGN);
   a->f.b.n1 = newId(yytext);
   lex();
   if(symb!=ASSIGN)
      error("assign",":= expected");
  
   lex();
   a->f.b.n2 = expr();
   return a;

}

NODE * ifComm(){
   extern NODE * condexp();
   NODE * t;
   NODE * c;
   c = newNode(IF);
   c->f.b.n1 = condexp();
   if(symb!=THEN)
      error("if","then expected");
   lex();
   t = commands();
   if(symb==ELSE){
      lex();
      c->f.b.n2 = newNode(ELSE);
      c->f.b.n2->f.b.n1 = t;
      c->f.b.n2->f.b.n2=commands();
   }
   else{
     c->f.b.n2 =t;
   }
   if(symb!=ENDIF)
      error("if","end if expected");
   lex();
   return c;
}

NODE * whileComm(){
   extern NODE * condexp();
   NODE * w;
   NODE * c;
   c = condexp();
   if(symb!=LOOP)
      error("while","loop expected");
   lex();
   w = newNode(WHILE);
   w->f.b.n1=c;
   w->f.b.n2=commands();
   if(symb!=ENDLOOP)
      error("while","end loop expected");
   lex();
   return w;
}

NODE * read(){
   NODE * r;
   r = newNode(READ);
   if(symb!=ID)
      error("read","ID expected");
   r->f.b.n1 = newId(yytext);
   lex();
   return r;
}

NODE * write(){
   extern NODE * expr();
   NODE * w;
   w = newNode(WRITE);
   w->f.b.n1=expr();
   return w;

}

NODE * condexp(){
   extern NODE * exprs();
   NODE * c;
   if(symb==LT||symb==LTE||symb==EQ||symb==NEQ){
      c = newNode(SEMI);
      c->f.b.n1=exprs();
   }
   else{
      error("condexp","Less,LessEq,Eq,NEq expected");
   }
   return c;
}

NODE * exprs(){
   extern NODE * expr();
   NODE * ex;
   ex = newNode(RBRA);
   ex->f.b.n1=expr();
   
   if(symb==COMMA){
      lex();
      ex->f.b.n2 = newNode(COMMA);
      ex->f.b.n2->f.b.n1=exprs();
   }
   return ex;
}

NODE * expr(){
   extern NODE * exprs();
   extern NODE * bop();
   NODE * e;
   switch(symb)
   {  
      case LT:
      case LTE:
      case EQ:
      case NEQ:
      case PLUS:
      case DIVIDE:
      case MINUS:
      case TIMES: e = bop();
                  break;
      case ID:    e = newNode(SEMI);
                  e->f.b.n1=newId(yytext);
                  lex();
                  if(symb==LBRA){
                     lex();
                     e->f.b.n2 = exprs();
                     if(symb!=RBRA)
                        error("expr",") expected");
                     lex();
                  }
                  break;

      case INT:   e = newInt(atoi(yytext));
                  lex();
                  break;

      default: error("expr","Plus/Divide/Minus/Times/ID/INT expected");
   }
   return e;
}

NODE * bop(){
   extern NODE * expr();
   NODE *b;
   b=newNode(symb);
   lex();
   if(symb!=LBRA)
      error("expr","( expected");
   
   lex();
   b->f.b.n1=expr();
   if(symb!=COMMA)
      error("expr",", expected");
   lex();
   b->f.b.n2=expr();
   if(symb!=RBRA)
      error("expr",") expected");
   lex();
   return b;
}

main(int c, char ** argv){
	
	if((yyin=fopen(argv[1],"r"))==NULL){
		printf("cant't open %s\n",argv[1]);
		exit(0);
	}
   
   	symb = yylex();
   	NODE* ast = program();
   	showTree(ast,1);
   	fclose(yyin);
}




