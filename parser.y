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



%type <op>  mult_op
            add_op
            shift_op
            relational_op
            equality_op
            assignment_op

%type <astnode_p>   primary_expr
                    postfix_expr
                    subscript_expr
                    component_selection_expr
                    direct_component_selection
                    indirect_component_selection
                    function_call
                    expr_list
                    postincrement_expr
                    postdecrement_expr
                    /* compound_literal */
                    cast_expr
                    unary_expr
                    sizeof_expr
                    unary_minus_expr
                    unary_plus_expr
                    logical_negation_expr
                    bitwise_negation_expr
                    address_expr
                    indirection_expr
                    preincrement_expr
                    predecrement_expr
                    multiplicative_expr
                    additive_expr
                    shift_expr
                    relational_expr
                    equality_expr
                    bitwise_or_expr
                    bitwise_xor_expr
                    bitwise_and_expr
                    logical_or_expr
                    logical_and_expr
                    conditional_expr
                    assignment_expr
                    comma_expr
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


statement: expr_statement           
         | statement expr_statement 
         ;

expr_statement: expr ';'        { print_ast($1); }
              | /* opt */ ';'   
              ;

expr: comma_expr    { $$ = $1; }
    ;

comma_expr: assignment_expr                 { $$ = $1; }
          | comma_expr ',' assignment_expr  {
              $$ = alloc_astnode_binary(',', $1, $3);
          }

primary_expr: IDENT         { $$ = alloc_astnode_ident($1); }
            | NUMBER        { $$ = alloc_astnode_number($1); }
            | CHARLIT       { $$ = alloc_astnode_charlit($1); }
            | STRING        { $$ = alloc_astnode_string($1); }
            | '(' expr ')'  { $$ = $2; }
            ;

postfix_expr: primary_expr              { $$ = $1; }
            | subscript_expr            { $$ = $1; }
            | component_selection_expr  { $$ = $1; }
            | function_call             {}
            | postincrement_expr        { $$ = $1; }
            | postdecrement_expr        { $$ = $1; }
            //| compound_literal                          {}
            ;

subscript_expr  : postfix_expr '[' expr ']' {
                    $$ = alloc_astnode_binary('+', $1, $3);
                    $$ = alloc_astnode_unary('*', $$);
                }
                ;

component_selection_expr: direct_component_selection    { $$ = $1; }
                        | indirect_component_selection  { $$ = $1; }
                        ;

direct_component_selection  : postfix_expr '.' IDENT    {
                                $$ = alloc_astnode_select($1, $3);
                            }
                            ;

indirect_component_selection: postfix_expr INDSEL IDENT {
                                $$ = alloc_astnode_unary('*', $1);
                                $$ = alloc_astnode_select($$, $3);
                            }
                            ;

function_call   : postfix_expr '(' expr_list ')'    {}
                | postfix_expr '(' ')'              {}
                ;

expr_list   : assignment_expr               { $$ = $1; }
            | expr_list ',' assignment_expr {
                $$ = alloc_astnode_binary(',', $1, $3);
            }
            ;

postincrement_expr  : postfix_expr PLUSPLUS {
                        $$ = alloc_astnode_unary(PLUSPLUS, $1);
                    }
                    ;
postdecrement_expr  : postfix_expr MINUSMINUS   {
                        $$ = alloc_astnode_unary(MINUSMINUS, $1);
                    }
                    ;

/*
compound_literal: '(' type_name ')' '{' init_list '}'       {}
                | '(' type_name ')' '{' init_list ',' '}'   {}
                ;
*/

cast_expr   : unary_expr                   { $$ = $1; }
            //| '(' type_name ')' cast_expr  {}
            ;

unary_expr  : postfix_expr          { $$ = $1; }
            | sizeof_expr           { $$ = $1; }
            | unary_minus_expr      { $$ = $1; }
            | unary_plus_expr       { $$ = $1; }
            | logical_negation_expr { $$ = $1; }
            | bitwise_negation_expr { $$ = $1; }
            | address_expr          { $$ = $1; }
            | indirection_expr      { $$ = $1; }
            | preincrement_expr     { $$ = $1; }
            | predecrement_expr     { $$ = $1; }
            ;

sizeof_expr : SIZEOF unary_expr {}
            //| SIZEOF '(' type_name ')' {}
            ;

unary_minus_expr: '-' cast_expr { $$ = alloc_astnode_unary('-', $2); }
                ;

unary_plus_expr : '+' cast_expr { $$ = alloc_astnode_unary('+', $2); }
                ;

logical_negation_expr   : '!' cast_expr { $$ = alloc_astnode_unary('!', $2); }
                        ;

bitwise_negation_expr   : '~' cast_expr { $$ = alloc_astnode_unary('~', $2); }
                        ;

address_expr    : '&' cast_expr { $$ = alloc_astnode_unary('&', $2); }
                ;

indirection_expr    : '*' cast_expr { $$ = alloc_astnode_unary('*', $2); }
                    ;

preincrement_expr   : PLUSPLUS unary_expr   {
                        number_t one = {
                            .integer = 1,
                            .type = NUM_INT
                        };
                        $$ = alloc_astnode_number(one);
                        $$ = alloc_astnode_binary(PLUSEQ, $2, $$);
                    }
                    ;

predecrement_expr   : MINUSMINUS unary_expr {
                        number_t one = {
                            .integer = 1,
                            .type = NUM_INT
                        };
                        $$ = alloc_astnode_number(one);
                        $$ = alloc_astnode_binary(MINUSEQ, $2, $$);
                    }
                    ;

multiplicative_expr : cast_expr                             { $$ = $1; }
                    | multiplicative_expr mult_op cast_expr {
                        $$ = alloc_astnode_binary($2, $1, $3);
                    }
                    ;

mult_op : '*'   { $$ = '*'; }
        | '/'   { $$ = '/'; }
        | '%'   { $$ = '%'; }
        ;

additive_expr   : multiplicative_expr                       { $$ = $1; }
                | additive_expr add_op multiplicative_expr  {
                    $$ = alloc_astnode_binary($2, $1, $3);
                }
                ;

add_op  : '+'   { $$ = '+'; }
        | '-'   { $$ = '-'; }
        ;

shift_expr  : additive_expr                     { $$ = $1; }
            | shift_expr shift_op additive_expr {
                $$ = alloc_astnode_binary($2, $1, $3);
            }
            ;

shift_op: SHL   { $$ = SHL; }
        | SHR   { $$ = SHR; }
        ;

relational_expr : shift_expr                                { $$ = $1; }
                | relational_expr relational_op shift_expr  {
                    $$ = alloc_astnode_binary($2, $1, $3);
                }
                ;

relational_op   : '<'   { $$ = '<';  }
                | '>'   { $$ = '>';  }
                | LTEQ  { $$ = LTEQ; }
                | GTEQ  { $$ = GTEQ; }
                ;

equality_expr   : relational_expr                           { $$ = $1; }
                | equality_expr equality_op relational_expr {
                    $$ = alloc_astnode_binary($2, $1, $3);
                }
                ;

equality_op : EQEQ  { $$ = EQEQ;  }
            | NOTEQ { $$ = NOTEQ; }
            ;

bitwise_or_expr : bitwise_xor_expr                      { $$ = $1; }
                | bitwise_or_expr '|' bitwise_xor_expr  {
                    $$ = alloc_astnode_binary('|', $1, $3);
                }
                ;

bitwise_xor_expr: bitwise_and_expr                      { $$ = $1; }
                | bitwise_xor_expr '^' bitwise_and_expr {
                    $$ = alloc_astnode_binary('^', $1, $3);
                }
                ;

bitwise_and_expr: equality_expr                         { $$ = $1; }
                | bitwise_and_expr '&' equality_expr    {
                    $$ = alloc_astnode_binary('&', $1, $3);
                }
                ;

logical_or_expr : logical_and_expr                          { $$ = $1; }
                | logical_or_expr LOGOR logical_and_expr    {
                    $$ = alloc_astnode_binary(LOGOR, $1, $3);
                }
                ;

logical_and_expr: bitwise_or_expr                           { $$ = $1; }
                | logical_and_expr LOGAND bitwise_or_expr   {
                    $$ = alloc_astnode_binary(LOGAND, $1, $3);
                }
                ;

conditional_expr: logical_or_expr                               { $$ = $1; }
                | logical_or_expr '?' expr ':' conditional_expr { 
                    $$ = alloc_astnode_ternary($1, $3, $5);
                }
                ;

assignment_expr : conditional_expr                          { $$ = $1; }
                | unary_expr assignment_op assignment_expr  {
                     $$ = alloc_astnode_binary($2, $1, $3);
                }
                ;

assignment_op   : '='       { $$ = '=';     }
                | TIMESEQ   { $$ = TIMESEQ; }
                | DIVEQ     { $$ = DIVEQ;   }
                | MODEQ     { $$ = MODEQ;   }
                | PLUSEQ    { $$ = PLUSEQ;  }
                | MINUSEQ   { $$ = MINUSEQ; }
                | SHLEQ     { $$ = SHLEQ;   }
                | SHREQ     { $$ = SHREQ;   }
                | ANDEQ     { $$ = ANDEQ;   }
                | XOREQ     { $$ = XOREQ;   }
                | OREQ      { $$ = OREQ;    }
                ;


%%

void yyerror(char const *s) {
    fprintf(stderr, "Error: %s\n", s);
}
