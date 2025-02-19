%{
    #include <stdio.h>
    #include <stdlib.h>

    extern int yylex(void);
    void yyerror(char const *);
%}

%code requires {
    #include "manual.h"
}

%token IDENT CHARLIT STRING NUMBER INDSEL PLUSPLUS MINUSMINUS SHL SHR LTEQ GTEQ
%token EQEQ NOTEQ LOGAND LOGOR ELLIPSIS TIMESEQ DIVEQ MODEQ PLUSEQ MINUSEQ SHLEQ
%token SHREQ ANDEQ OREQ XOREQ AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO
%token DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INLINE INT LONG REGISTER
%token RESTRICT RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION
%token UNSIGNED VOID VOLATILE WHILE _BOOL _COMPLEX _IMAGINARY

%union {
    number_t num;
    string_t str;
    char char_literal;
}

%left '+'

%%
expr:   NUMBER
    |   expr '+' expr
    ;

%%

void yyerror(char const *s) {
    fprintf(stderr, "Error: %s\n", s);
}
