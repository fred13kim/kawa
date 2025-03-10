#ifndef AST_H_
#define AST_H_

#include "parser_defs.h"


typedef struct astnode_s astnode_t;

astnode_t *alloc_astnode_binop(int op, astnode_t *left, astnode_t *right);
astnode_t *alloc_astnode_ident(string_t ident);
astnode_t *alloc_astnode_number(number_t number);
astnode_t *alloc_astnode_charlit(char charlit);
astnode_t *alloc_astnode_string(string_t string);

void print_ast(astnode_t *node);

enum nodetype{
    AST_BINOP,
    AST_IDENT,
    AST_NUM,
    AST_CHARLIT,
    AST_STRING,
};

struct astnode_binary_s {
    int op;
    astnode_t *left;
    astnode_t *right;
};

struct astnode_ident_s {
    string_t str;
};

struct astnode_num_s {
    number_t num;
};

struct astnode_charlit_s {
    char char_literal;
};

struct astnode_string_s {
    string_t str;
};

struct astnode_s {
    enum nodetype type;
    union {
        struct astnode_binary_s  binary;
        struct astnode_ident_s   ident;
        struct astnode_num_s     num;
        struct astnode_charlit_s charlit;
        struct astnode_string_s  str;
    };
};


#endif // !AST_H_
