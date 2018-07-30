#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

int symb;
extern printSymb();
extern FILE* yyin;

lex(){
	printSymb();
	symb = yylex();
}

error(char * rule,char* message){  
	printf("%s: found %s\n",rule,showSymb(symb));
	printf("%s: %s\n",rule,message); exit(0); 
}

rule(char * name,int depth){
	int i;
	for(i=0;i<depth;i++)
		printf(" ");
	printf("%s\n",name);
}

program(int depth){
	rule("program",depth);
	if(symb==FUNCTION)
		functions(depth+1);

}

functions(int depth){
	rule("functions",depth);
	function(depth + 1);
	if(symb==SEMI)
	{
		lex();
		if(symb==FUNCTION){
			functions(depth+1);
		}
	}
	else{
		error("functions","; expected");
	}	
}

function(int depth){
	rule("function",depth);
	if(symb!=FUNCTION)
		error("function","function expected");
	lex();
	if(symb!=ID && symb!=MAIN)
		error("function","name expected");
	if(symb==MAIN){
		lex();
		if(symb!=LBRA)
			error("function","( expected");
		lex();
		if(symb!=RBRA)
			error("function",") expected");
		lex();
	}
	else{
		lex();
		if(symb!=LBRA)
			error("function","( expected");
		lex();
		if(symb==ID){
			args(depth+1);
		}
		if(symb!=RBRA)
			error("function",") expected");
		lex();
		if(symb==RETURNS){
			lex();
			if(symb!=ID)
				error("function","name expected");
		lex();
		}
	}
	if(symb==VARIABLES){
		lex();
		args(depth+1);
	}
	if(symb!=TBEGIN)
		error("function","begin expected");
	lex();
	commands(depth+1);
	if(symb!=ENDFUNCTION)
		error("function","end function expected");
	lex();
}

args(int depth){
	rule("args",depth);
	if(symb!=ID)
		error("args","name expected");
	lex();
	if(symb==COMMA){
		lex();
		args(depth+1);
	}
}

commands(int depth){
	rule("commands",depth);
	command(depth+1);
	if(symb!=SEMI)
		error("commands","; expected");
	lex();
	if(symb==ID||symb==IF||symb==WHILE||symb==READ||symb==WRITE){
		commands(depth+1);
	}
}

command(depth){
	rule("command",depth);
	switch(symb)
	{
		case ID: 	assign(depth+1);
				 	break;
		case IF: 	lex();
				 	ifComm(depth+1);
				 	break;
		case WHILE: lex();
					whileComm(depth+1);
					break;
		case READ:	lex();
					read(depth+1);
					break;
		case WRITE: lex();
					write(depth+1);
					break;

		default: error("command","name/if/while/read/write expected");


	}

}

assign(int depth){
	rule("assign",depth);
	lex();
	if(symb!=ASSIGN)
		error("assign",":= expected");
	lex();
	expr(depth+1);

}

ifComm(int depth){
	rule("if",depth);
	condexp(depth+1);
	if(symb!=THEN)
		error("if","then expected");
	lex();
	commands(depth+1);
	if(symb==ELSE){
		lex();
		commands(depth+1);
	}
	if(symb!=ENDIF)
		error("if","end if expected");
	lex();

}

whileComm(int depth){
	rule("while",depth);
	condexp(depth+1);
	if(symb!=LOOP)
		error("while","loop expected");
	lex();
	commands(depth+1);
	if(symb!=ENDLOOP)
		error("while","end loop expected");
	lex();
}

read(int depth){
	rule("read",depth);
	if(symb!=ID)
		error("read","ID expected");
	lex();
}

write(int depth){
	rule("write",depth);
	expr(depth+1);

}

condexp(int depth){
	rule("condexp",depth);
	if(symb==LT||symb==LTE||symb==EQ||symb==NEQ){
		exprs(depth+1);
	}
	else{
		error("condexp","Less,LessEq,Eq,NEq expected");
	}

}

exprs(int depth){
	rule("exprs",depth);
	expr(depth+1);
	
	if(symb==COMMA){
		lex();
		exprs(depth+1);
	}

}

expr(int depth){
	rule("expr",depth);
	switch(symb)
	{	
		case LT:
		case LTE:
		case EQ:
		case NEQ:
		case PLUS:
		case DIVIDE:
		case MINUS:
		case TIMES:		lex();
				 		bop(depth+1);
				 		break;

		case ID: 		lex();
						if(symb==LBRA){
				 			lex();
				 			exprs(depth+1);
				 			if(symb!=RBRA)
				 				error("expr",") expected");
				 			lex();
				 		}
				 		break;

		case INT: 		lex();
						break;

		default: error("expr","Plus/Divide/Minus/Times/ID/INT expected");
	}
}

bop(int depth){
	rule("bop",depth);
	if(symb!=LBRA)
		error("expr","( expected");
	lex();
	expr(depth+1);
	if(symb!=COMMA)
		error("expr",", expected");
	lex();
	expr(depth+1);
	if(symb!=RBRA)
		error("expr",") expected");
	lex();
}

main(int c, char ** argv){
	
	if((yyin=fopen(argv[1],"r"))==NULL){
		printf("cant't open %s\n",argv[1]);
		exit(0);
	}
	symb = yylex();
	program(1);
	fclose(yyin);



}
