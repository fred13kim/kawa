CC = gcc
LEX = flex


lexertester: lexer.o lexertester.o
	$(CC) -o lexertester lexer.o lexertester.o

lexertester.o: lexertester.c tokens-manual.h
	$(CC) -c lexertester.c

lexer.c: lexer.l
	$(LEX) --header-file=lexer.h lexer.l

lexer.o: lexer.c tokens-manual.h
	$(CC) -c lexer.c

test: lexertester
	$(CC) -E ./ltests/*.c | ./lexertester

debug: lexertester
	$(CC) -g -o lexertester lexer.c lexertester.c

clean:
	rm -f lexertester *.o lexer.c lexer.h
