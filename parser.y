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

%union {
    int      op;
    number_t num;
    string_t str;
    char char_literal;
    astnode_t *astnode_p;
}

%token IDENT CHARLIT STRING NUMBER INDSEL PLUSPLUS MINUSMINUS SHL SHR LTEQ GTEQ
    EQEQ NOTEQ LOGAND LOGOR ELLIPSIS TIMESEQ DIVEQ MODEQ PLUSEQ MINUSEQ SHLEQ
    SHREQ ANDEQ OREQ XOREQ AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE
    ELSE ENUM EXTERN FLOAT FOR GOTO IF INLINE INT LONG REGISTER RESTRICT RETURN
    SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID
    VOLATILE WHILE _BOOL _COMPLEX _IMAGINARY




%type <astnode_p>   expr
                    assign_expr
                    cond_expr

%type <op>    assign_op


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

stmt:   expr ';'    { print_ast($1); }
    |   stmt expr   { print_ast($2); }
    ;
expr:   assign_expr { $$ = $1; }
    |   expr ',' assign_expr    { alloc_astnode_binop(',',$1,$3); }
    ;

cond_expr:      {}

unary_expr:     {}

assign_expr:    cond_expr   { $$ = $1; }
    |   unary_expr assign_op assign_expr    {  }
    ;

assign_op:  '='     { $$ = '=';     }
         |  TIMESEQ { $$ = TIMESEQ; }
         |  DIVEQ   { $$ = DIVEQ;   }
         |  MODEQ   { $$ = MODEQ;   }
         |  PLUSEQ  { $$ = PLUSEQ;  }
         |  MINUSEQ { $$ = MINUSEQ; }
         |  SHLEQ   { $$ = SHLEQ;   }
         |  SHREQ   { $$ = SHREQ;   }
         |  ANDEQ   { $$ = ANDEQ;   }
         |  XOREQ   { $$ = XOREQ;   }
         |  OREQ    { $$ = OREQ;    }


%%

void yyerror(char const *s) {
    fprintf(stderr, "Error: %s\n", s);
}
