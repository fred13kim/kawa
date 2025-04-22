#ifndef AST_H_
#define AST_H_

#include "parser_defs.h"


typedef struct astnode_s astnode_t;

astnode_t *alloc_astnode_unary(int op, astnode_t *expr);
astnode_t *alloc_astnode_binary(int op, astnode_t *expr1, astnode_t *expr2);
astnode_t *alloc_astnode_ternary(astnode_t *expr1, astnode_t *expr2, astnode_t *expr3);
astnode_t *alloc_astnode_select(astnode_t *expr, string_t ident);
astnode_t *alloc_astnode_fncall(astnode_t *name, astnode_t *args);
astnode_t *alloc_astnode_arg_list(astnode_t *head);
astnode_t *alloc_astnode_arg_node(astnode_t *arg);
astnode_t *append_astnode(astnode_t *list, astnode_t *node);
astnode_t *alloc_astnode_sizeof(astnode_t *expr);
astnode_t *alloc_astnode_ident(string_t ident);
astnode_t *alloc_astnode_number(number_t number);
// astnode_t *alloc_astnode_charlit(char charlit);
astnode_t *alloc_astnode_string(string_t string);

void print_ast(astnode_t *node);

enum nodetype{
    AST_UNARY,
    AST_BINARY,
    AST_TERNARY,
    AST_SELECT,
    AST_FNCALL,
    AST_ARG_LIST,
    AST_ARG_NODE,
    AST_SIZEOF,
    AST_IDENT,
    AST_NUM,
    AST_CHARLIT,
    AST_STRING,
};

struct astnode_unary_s {
    int op;
    astnode_t *expr;
};

struct astnode_binary_s {
    int op;
    astnode_t *expr1;
    astnode_t *expr2;
};

struct astnode_ternary_s {
    astnode_t *expr1;
    astnode_t *expr2;
    astnode_t *expr3;
};

struct astnode_select_s {
    astnode_t *expr;
    astnode_t *ident;
};

struct astnode_fncall_s {
    astnode_t *name;
    astnode_t *args;
};

struct astnode_arg_list_s {
    int size;
    astnode_t *head;
    astnode_t *tail;
};

struct astnode_arg_node_s {
    astnode_t *arg;
    astnode_t *next;
};

struct astnode_sizeof_s {
    astnode_t *expr;
};

struct astnode_ident_s {
    string_t str;
};

struct astnode_num_s {
    number_t num;
};

// struct astnode_charlit_s {
//     char char_literal;
// };

struct astnode_string_s {
    string_t str;
};

struct astnode_s {
    enum nodetype type;
    union {
        struct astnode_unary_s      unary;
        struct astnode_binary_s     binary;
        struct astnode_ternary_s    ternary;
        struct astnode_select_s     select;
        struct astnode_fncall_s     fncall;
        struct astnode_arg_list_s   arg_list;
        struct astnode_arg_node_s   arg_node;
        struct astnode_sizeof_s     _sizeof;
        struct astnode_ident_s      ident;
        struct astnode_num_s        num;
        // struct astnode_charlit_s    charlit;
        struct astnode_string_s     str;
    };
};


#endif // !AST_H_
