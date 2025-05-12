#ifndef AST_H_
#define AST_H_

#include "parser_defs.h"


typedef struct astnode_s astnode_t;

astnode_t *alloc_astnode_unary(int op, astnode_t *expr);
astnode_t *alloc_astnode_binary(int op, astnode_t *expr1, astnode_t *expr2);
astnode_t *alloc_astnode_ternary(astnode_t *expr1, astnode_t *expr2, astnode_t *expr3);
astnode_t *alloc_astnode_select(astnode_t *expr, string_t ident);
astnode_t *alloc_astnode_fncall(astnode_t *name, astnode_t *args);

void *free_astnode_ll_list(astnode_t *list);
astnode_t *alloc_astnode_ll_list(astnode_t *head); // linked list list
astnode_t *alloc_astnode_ll_node(astnode_t *arg); // linked list node
astnode_t *append_astnode(astnode_t *list, astnode_t *node);
astnode_t *append_astnode_ll_node(astnode_t *list, astnode_t *ll_node);

/* not sure if i will need these prepennd functions but let's keep them for now */
astnode_t *prepend_astnode(astnode_t *list, astnode_t *node);
astnode_t *prepend_astnode_ll_node(astnode_t *list, astnode_t *ll_node);
astnode_t *append_astlist(astnode_t *list1, astnode_t *list2);
astnode_t *reduce_astlist(astnode_t *list);

astnode_t *alloc_astnode_sizeof(astnode_t *expr);
astnode_t *alloc_astnode_ident(string_t ident);
astnode_t *alloc_astnode_number(number_t number);
// astnode_t *alloc_astnode_charlit(char charlit);
astnode_t *alloc_astnode_string(string_t string);
astnode_t *alloc_astnode_declaration(astnode_t *declaration_spec_list, astnode_t *init_declarator_list);
astnode_t *alloc_astnode_declaration_spec(int spec, int spec_type);

astnode_t *alloc_astnode_ptr(astnode_t *ptr_to);
astnode_t *alloc_astnode_array(astnode_t *ptr_to, astnode_t *size);
astnode_t *alloc_astnode_func(astnode_t *name, astnode_t *args);


enum nodetype{
    AST_UNARY,
    AST_BINARY,
    AST_TERNARY,
    AST_SELECT,
    AST_FNCALL,
    AST_LL_LIST,
    AST_LL_NODE,
    AST_SIZEOF,
    AST_IDENT,
    AST_NUM,
    AST_CHARLIT,
    AST_STRING,

    AST_DECLARATION,
    AST_DECLARATION_SPEC,

    AST_PTR,
    AST_ARRAY,
    AST_FUNC,
};

enum declaration_specifiers {
    STORAGE_CLASS,
    TYPE_SPECIFIER,
    TYPE_QUALIFIER,
    FUNC_SPECIFIER,
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

struct astnode_ll_list_s {
    int size;
    astnode_t *head;
    astnode_t *tail;
};

struct astnode_ll_node_s {
    astnode_t *node;
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

struct astnode_declaration_s {
    astnode_t *declaration_spec_list;
    astnode_t *init_declarator_list;
};

struct astnode_declaration_spec_s {
    int spec_type;  /* either STORAGE_CLASS, TYPE_SPECIFIER, TYPE_QUALIFIER, FUNC_SPECIFIER */
    int spec;       /* the actual specifier itself ex. typedef (i am not going to implement this tho T_T )*/
};

struct astnode_ptr_s {
    astnode_t *ptr_to;
};

struct astnode_array_s {
    astnode_t *ptr_to;
    astnode_t *size;
};

struct astnode_func_s {
    astnode_t *name;
    astnode_t *args;
    astnode_t *ret_type;
};

struct astnode_s {
    enum nodetype type;
    union {
        struct astnode_unary_s      unary;
        struct astnode_binary_s     binary;
        struct astnode_ternary_s    ternary;
        struct astnode_select_s     select;
        struct astnode_fncall_s     fncall;
        struct astnode_ll_list_s    ll_list;
        struct astnode_ll_node_s    ll_node;
        struct astnode_sizeof_s     _sizeof;
        struct astnode_ident_s      ident;
        struct astnode_num_s        num;
        // struct astnode_charlit_s    charlit;
        struct astnode_string_s     str;
        struct astnode_declaration_s    declaration;
        struct astnode_declaration_spec_s declaration_spec;
        struct astnode_ptr_s        ptr;
        struct astnode_array_s      array;
        struct astnode_func_s       func;
    };
};


#endif // !AST_H_
