#ifndef AST_H_
#define AST_H_

#include "parser_defs.h"

typedef struct astnode_s astnode_t;

enum nodetype{
    AST_CHARLIT,
    AST_STRING,
    AST_NUM,
    AST_IDENT,
    AST_BINOP,
};

struct astnode_binary {
    int op;
    astnode_t *left;
    astnode_t *right;
};

struct astnode_charlit {
    char char_literal;
};

struct astnode_string {
    string_t str;
};

struct astnode_num {
    number_t num;
};

struct astnode_ident {
    string_t str;
};

struct astnode_s {
    enum nodetype type;
    union {
        struct astnode_binary  binary;
        struct astnode_charlit charlit;
        struct astnode_string  str;
        struct astnode_num     num;
        struct astnode_ident   ident;
    };
};


#endif // !AST_H_
