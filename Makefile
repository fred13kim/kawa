CC = gcc
LEX = flex


lexertester: lexer.c lexertester.o
	$(CC) -o lexertester lexer.c lexertester.o

lexer.c: lexer.l
	$(LEX) --header-file=lexer.h lexer.l

lexertester.o: lexertester.c tokens-manual.h
	$(CC) -c lexertester.c

test: lexertester
	$(CC) -E ./ltests/*.c | ./lexertester

debug: lexertester
	$(CC) -g -o lexertester lexer.c lexertester.c

clean:
	rm -f lexertester *.o lexer.c lexer.h
