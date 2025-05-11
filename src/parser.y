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

/* External Defs */
%type <astnode_p>   translation_unit
                    external_declaration
                    function_definition
                    declaration_list_opt
                    declaration_list

/* Expressions */
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
                    assignment_expr_opt
                    assignment_expr
                    comma_expr
                    expr_opt
                    expr
                    constant_expr

/* Declarations */
%type <astnode_p>   declaration
                    declaration_specifiers_opt
                    declaration_specifiers
                    init_declarator_list
                    init_declarator
                    storage_class_specifier
                    type_specifier
                    type_qualifier
                    function_specifier
                    declarator
                    direct_declarator
                    pointer
                    type_qualifier_list_opt
                    type_qualifier_list
                    parameter_type_list_opt
                    parameter_type_list
                    parameter_list
                    parameter_declaration
                    identifier_list_opt
                    identifier_list
                    /* type_name */
                    abstract_declarator
                    direct_abstract_declarator_opt
                    direct_abstract_declarator
                    /* typedef_name */
                    /* initializer  */
                    /* initializer_list */
                    /* designation */
                    /* designator_list  */
                    /* designator */

/* Statements */
%type <astnode_p>   statement
                    labeled_statement
                    compound_statement
                    block_item_list
                    block_item
                    expr_statement
                    selection_statement
                    iteration_statement
                    jump_statement


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

/* A.2.4 External Definitions */

translation_unit: external_declaration
                | translation_unit external_declaration
                ;

external_declaration: function_definition
                    | declaration
                    ;

function_definition : declaration_specifiers declarator declaration_list_opt compound_statement
                    ;

declaration_list_opt: declaration_list  { $$ = $1; }
                    | /* opt */         { $$ = NULL; }
                    ;

declaration_list: declaration                   {}
                | declaration_list declaration  { /* APPEND SHIT */ }
                ;

/* A.2.1 Expressions */


primary_expr: IDENT         { $$ = alloc_astnode_ident($1); }
            | NUMBER        { $$ = alloc_astnode_number($1); }
            | CHARLIT       { 
                number_t num  = {
                    .integer = $1,
                    .type = NUM_UINT,
                };
                $$ = alloc_astnode_number(num);
            }
            | STRING        { $$ = alloc_astnode_string($1); }
            | '(' expr ')'  { $$ = $2; }
            ;

postfix_expr: primary_expr              { $$ = $1; }
            | subscript_expr            { $$ = $1; }
            | component_selection_expr  { $$ = $1; }
            | function_call             { $$ = $1; }
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

function_call   : postfix_expr '(' expr_list ')'    {
                    $$ = alloc_astnode_fncall($1, $3);
                }
                | postfix_expr '(' ')'              {
                    $$ = alloc_astnode_fncall($1,NULL);
                }
                ;

expr_list   : assignment_expr               {
                $$ = alloc_astnode_arg_node($1);
                $$ = alloc_astnode_arg_list($$);
            }
            | expr_list ',' assignment_expr { $$ = append_astnode($1, $3); }
            ;

postincrement_expr  : postfix_expr PLUSPLUS {
                        $$ = alloc_astnode_unary(PLUSPLUS, $1);
                    }
                    ;
postdecrement_expr  : postfix_expr MINUSMINUS   {
                        $$ = alloc_astnode_unary(MINUSMINUS, $1);
                    }
                    ;

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

sizeof_expr : SIZEOF unary_expr { $$ = alloc_astnode_sizeof($2); }
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

assignment_expr_opt : assignment_expr   { $$ = $1; }
                    | /* opt */         { $$ = NULL; }
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

comma_expr  : assignment_expr                 { $$ = $1; }
            | comma_expr ',' assignment_expr  {
                $$ = alloc_astnode_binary(',', $1, $3);
            }
            ;

expr_opt: expr      { $$ = $1;  }
        | /* opt */ { $$ = NULL; }
        ;

expr: comma_expr    { $$ = $1; }
    ;

constant_expr   : conditional_expr  { $$ = $1; }
                ;

/* A.2.2 Declarations */

declaration : declaration_specifiers init_declarator_list ';'   {
                $$ = alloc_astnode_declaration($1, $2);
            }
            | declaration_specifiers ';'                        {
                $$ = alloc_astnode_declaration($1, NULL); 
            }
            ;


declaration_specifiers  : storage_class_specifier declaration_specifiers    {}
                        | storage_class_specifier                           { $$ = $1; }
                        | type_specifier declaration_specifiers
                        | type_specifier
                        | type_qualifier declaration_specifiers
                        | type_qualifer
                        | function_specifier declaration_specifiers
                        | function_specifier
                        ;

init_declarator_list: init_declarator
                    | init_declarator_list ',' init_declarator
                    ;

init_declarator : declarator
                //| declarator '=' initializer
                ;

storage_class_specifier : TYPEDEF
                        | EXTERN
                        | STATIC
                        | AUTO      
                        | REGISTER  {}
                        ;

type_specifier  : VOID
                | CHAR
                | SHORT
                | INT
                | LONG
                | FLOAT
                | DOUBLE
                | SIGNED
                | UNSIGNED
                | _BOOL
                | _COMPLEX
                //| struct_or_union_specifier   i fear
                //| enum_specifier              i am
                //| typedef_name                joever & cooked
                ;

type_qualifier  : CONST
                | RESTRICT
                | VOLATILE
                ;

function_specifier  : INLINE
                    ;

declarator  : pointer direct_declarator
            | direct_declarator
            ;

direct_declarator   : IDENT
                    | '{' declarator '}'
                    | direct_declarator '[' type_qualifier_list_opt assignment_expr_opt ']'
                    | direct_declarator '[' STATIC type_qualifier_list_opt assignment_expr ']'
                    | direct_declarator '[' type_qualifier_list STATIC assignment_expr ']'
                    | direct_declarator '[' type_qualifier_list_opt '*' ']'
                    | direct_declarator '(' parameter_type_list ')'
                    | direct_declarator '(' identifier_list_opt ')'
                    ;

pointer : '*' type_qualifier_list_opt
        | '*' type_qualifier_list_opt pointer
        ;

type_qualifier_list_opt : type_qualifier_list
                        | /* opt */
                        ;

type_qualifier_list : type_qualifier
                    | type_qualifier_list type_qualifier
                    ;

parameter_type_list_opt : parameter_type_list
                        | /* opt */
                        ;

parameter_type_list : parameter_list
                    | parameter_list ',' ELLIPSIS
                    ;

parameter_list  : parameter_declaration
                | parameter_list ',' parameter_declaration
                ;

parameter_declaration   : declaration_specifiers declarator
                        | declaration_specifiers abstract_declarator
                        | declaration_specifiers
                        ;

identifier_list_opt : identifier_list
                    | /* opt */
                    ;

identifier_list : IDENT
                | identifier_list ',' IDENT
                ;

abstract_declarator : pointer
                    | pointer direct_abstract_declarator
                    | direct_abstract_declarator
                    ;

direct_abstract_declarator_opt  : direct_abstract_declarator
                                | /* opt */
                                ;

direct_abstract_declarator  : '(' abstract_declarator ')'
                            | direct_abstract_declarator_opt '[' assignment_expr_opt ']'
                            | direct_abstract_declarator_opt '[' '*' ']'
                            | direct_abstract_declarator_opt '(' parameter_type_list_opt ')'
                            ;

/*
initializer : assignment_expr
            | '{' initializer_list '}'
            | '{' identifier_list ',' '}'
            ;

initializer_list: designation initializer
                | initializer
                | initializer_list ',' designation initializer
                | initializer_list ',' initializer
                ;

designation : designator_list '='
            ;

designator_list : designator
                | designator_list designator
                ;

designator  : '[' constant_expr ']'
            | '.' IDENT
            ;

*/


statement   : labeled_statement     { $$ = $1; }
            | compound_statement    { $$ = $1; }
            | expr_statement        { $$ = $1; }
            | selection_statement   { $$ = $1; }
            | iteration_statement   { $$ = $1; }
            | jump_statement        { $$ = $1; }
            ;

labeled_statement   : IDENT ':' statement
                    | CASE constant_expr ':' statement
                    | DEFAULT ':' statement
                    ;

compound_statement  : '{' block_item_list '}'   {
                        $$ = alloc_astnode_compound_statement();
                    }
                    | '{' '}'                   { $$ = NULL; }
                    ;

block_item_list : block_item_list block_item    {
                    // append
                }
                | block_item                    {
                    // create_new list
                }
                ;

block_item  : declaration   { $$ = $1; }
            | statement     { $$ = $1; }
            ;

expr_statement  : expr_opt ';'  { $$ = $1; }
                ;

selection_statement : IF '(' expr ')' statement
                    | IF '(' expr ')' statement ELSE statement
                    | SWITCH '(' expr ')' statement
                    ;

iteration_statement : WHILE '(' expr ')' statement
                    | DO statement WHILE '(' expr ')'
                    | FOR '(' expr_opt ';' expr_opt ';' expr_opt ')' statement
                    | FOR '(' declaration expr_opt ';' expr_opt ')' statement
                    ;

jump_statement  : GOTO IDENT
                | CONTINUE ';'
                | BREAK ';'         
                | RETURN expr_opt ';'
                ;








%%

void yyerror(char const *s) {
    fprintf(stderr, "Error: %s\n", s);
}
