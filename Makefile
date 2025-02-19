CC 		= gcc
LEX 	= flex
YACC 	= bison

all: parsertester lexertester

parsertester: parser.o lexer.o parsertester.o
	$(CC) -o parsertester parser.o lexer.o parsertester.o

parsertester.o: parsertester.c
	$(CC) -c parsertester.c

lexertester: lexer.o lexertester.o parser.h
	$(CC) -o lexertester lexer.o parser.o lexertester.o

lexertester.o: lexertester.c
	$(CC) -c lexertester.c

lexer.c: lexer.l parser.h
	$(LEX) --header-file=lexer.h lexer.l

lexer.o: lexer.c
	$(CC) -c lexer.c

parser.h parser.c: parser.y
	$(YACC) -v --defines=parser.h -o parser.c parser.y

parser.o: parser.c manual.h
	$(CC) -c parser.c

test: lexertester
	$(CC) -E ./tests/ltests/*.c | ./lexertester

clean:
	rm -f lexertester *.o lexer.c lexer.h parser.c parser.h parser.output parsertester
