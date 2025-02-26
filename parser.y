%{
    #include <stdio.h>
    #include <stdlib.h>

    extern int yylex(void);
    void yyerror(char const *);
%}

%code requires {
    #include "ast.h"
    #include "parser_defs.h"
}

%token IDENT CHARLIT STRING NUMBER INDSEL PLUSPLUS MINUSMINUS SHL SHR LTEQ GTEQ
    EQEQ NOTEQ LOGAND LOGOR ELLIPSIS TIMESEQ DIVEQ MODEQ PLUSEQ MINUSEQ SHLEQ
    SHREQ ANDEQ OREQ XOREQ AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE
    ELSE ENUM EXTERN FLOAT FOR GOTO IF INLINE INT LONG REGISTER RESTRICT RETURN
    SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID
    VOLATILE WHILE _BOOL _COMPLEX _IMAGINARY

%union {
    number_t num;
    string_t str;
    char char_literal;
    struct astnode *astnode_p;
}


%type <astnode_p> binary_expr
                ternary_expr
                unary_expr
                prefix_expr
                postfix_expr
                primary_expr

/* C REF MANUAL Sec 7.2 */
/* binary */
%left ','
%right '=' PLUSEQ MINUSEQ TIMESEQ DIVEQ MODEQ SHLEQ SHREQ ANDEQ XOREQ OREQ

/* ternary */
%right '?' ':'

/* binary */
%left LOGOR
%left LOGAND
%left '|'
%left '^'
%left '&'
%left EQEQ NOTEQ
%left '<' '>' LTEQ GTEQ
%left SHL SHR
%left '+' '-'
%left '*' '/' '%'

/* unary */
%right '!'
%right '~'
%right SIZEOF

/* postfix */
%left PLUSPLUS MINUSMINUS
%left INDSEL
%left '.'
%left '(' ')' '[' ']'






%%
expr:   NUMBER
    |   expr '+' expr
    ;

%%

void yyerror(char const *s) {
    fprintf(stderr, "Error: %s\n", s);
}
