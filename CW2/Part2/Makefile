
all: func.lex tokens.h parse.c
	flex func.lex
	gcc -c lex.yy.c
	gcc -c parse.c
	gcc -c print.c
	gcc -o parser print.o lex.yy.o parse.o

clean:
	rm -rf *.o lex.yy.c parser
