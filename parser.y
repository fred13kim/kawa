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

%token <str> IDENT 
%token <char_literal> CHARLIT 
%token <str> STRING 
%token <num> NUMBER 
%token <op> INDSEL PLUSPLUS MINUSMINUS SHL SHR LTEQ GTEQ EQEQ NOTEQ LOGAND LOGOR
    ELLIPSIS TIMESEQ DIVEQ MODEQ PLUSEQ MINUSEQ SHLEQ SHREQ ANDEQ OREQ XOREQ
    AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM EXTERN FLOAT
    FOR GOTO IF INLINE INT LONG REGISTER RESTRICT RETURN SHORT SIGNED SIZEOF
    STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE _BOOL
    _COMPLEX _IMAGINARY



%type <op>  unary_op
            assignment_op
%type <astnode_p>   primary_expr
                    postfix_expr
                    argument_expr_list
                    unary_expr
                    cast_expr
                    multiplicative_expr
                    additive_expr
                    shift_expr
                    relational_expr
                    equality_expr
                    AND_expr
                    exclusive_OR_expr
                    inclusive_OR_expr
                    logical_AND_expr
                    logical_OR_expr
                    conditional_expr
                    assignment_expr
                    expr
//                    constant_expr


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


statement: expr_statement           {  }
         | statement expr_statement {  }
         ;

expr_statement: expr ';'        {  }
              | /* opt */ ';'   {  }
              ;

expr: assignment_expr           {  }
    | expr ',' assignment_expr  {  }
    ;

primary_expr: IDENT         {}
            | NUMBER        {}
            | CHARLIT       {}
            | STRING        {}
            | '(' expr ')'  {}
            ;

postfix_expr: primary_expr                              {}
            | postfix_expr '[' expr ']'                 {}
            | postfix_expr '(' argument_expr_list ')'   {}
            | postfix_expr '(' ')'                      {}
            | postfix_expr '.' IDENT                    {}
            | postfix_expr INDSEL IDENT                 {}
            | postfix_expr PLUSPLUS                     {}
            | postfix_expr MINUSMINUS                   {}
            //| '(' type_name ')' '{' init_list '}'       {}
            //| '(' type_name ')' '{' init_list ',' '}'   {}
            ;

argument_expr_list: assignment_expr                         {}
                  | argument_expr_list ',' assignment_expr  {}
                  ;

unary_expr: postfix_expr                {}
          | PLUSPLUS unary_expr         {}
          | MINUSMINUS unary_expr       {}
          | unary_op cast_expr          {}
          | SIZEOF unary_expr           {}
          //| SIZEOF '(' type_name ')'    {}
          ;

unary_op: '&'   { $$ = '&'; }
        | '*'   { $$ = '*'; }
        | '+'   { $$ = '+'; }
        | '-'   { $$ = '-'; }
        | '~'   { $$ = '~'; }
        | '!'   { $$ = '!'; }
        ;

cast_expr: unary_expr                   {}
         //| '(' type_name ')' cast_expr  {}
         ;

multiplicative_expr: cast_expr                          {}
                   | multiplicative_expr '*' cast_expr  {}
                   | multiplicative_expr '/' cast_expr  {}
                   | multiplicative_expr '%' cast_expr  {}
                   ;

additive_expr: multiplicative_expr                      {}
             | additive_expr '+' multiplicative_expr    {}
             | additive_expr '-' multiplicative_expr    {}
             ;

shift_expr: additive_expr                   {}
          | shift_expr SHL additive_expr    {}
          | shift_expr SHR additive_expr    {}
          ;

relational_expr: shift_expr                         {}
               | relational_expr '<' shift_expr     {}
               | relational_expr '>' shift_expr     {}
               | relational_expr LTEQ shift_expr    {}
               | relational_expr GTEQ shift_expr    {}
               ;

equality_expr: relational_expr                      {}
             | equality_expr EQEQ relational_expr   {}
             | equality_expr NOTEQ relational_expr  {}
             ;


AND_expr: equality_expr                 {}
        | AND_expr '&' equality_expr    {}
        ;

exclusive_OR_expr: AND_expr                         {}
                 | exclusive_OR_expr '^' AND_expr   {}
                 ;

inclusive_OR_expr: exclusive_OR_expr                        {}
                 | inclusive_OR_expr '|' exclusive_OR_expr  {}
                 ;

logical_AND_expr: inclusive_OR_expr                         {}
                | logical_AND_expr LOGAND inclusive_OR_expr {}
                ;

logical_OR_expr: logical_AND_expr                       {}
               | logical_OR_expr LOGOR logical_AND_expr {}
               ;

conditional_expr: logical_OR_expr                               {}
                | logical_OR_expr '?' expr ':' conditional_expr {}
                ;

assignment_expr: conditional_expr                           {}
               | unary_expr assignment_op assignment_expr   {}
               ;

assignment_op: '='      { $$ = '=';     }
             | TIMESEQ  { $$ = TIMESEQ; }
             | DIVEQ    { $$ = DIVEQ;   }
             | MODEQ    { $$ = MODEQ;   }
             | PLUSEQ   { $$ = PLUSEQ;  }
             | MINUSEQ  { $$ = MINUSEQ; }
             | SHLEQ    { $$ = SHLEQ;   }
             | SHREQ    { $$ = SHREQ;   }
             | ANDEQ    { $$ = ANDEQ;   }
             | XOREQ    { $$ = XOREQ;   }
             | OREQ     { $$ = OREQ;    }
             ;


%%

void yyerror(char const *s) {
    fprintf(stderr, "Error: %s\n", s);
}
