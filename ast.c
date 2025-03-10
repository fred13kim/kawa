#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

astnode_t *alloc_astnode(enum nodetype t) {
    astnode_t *node = malloc(sizeof(astnode_t));
    node->type = t;
    return node;
}

astnode_t *alloc_astnode_binop(int op, astnode_t *left, astnode_t *right) {
    astnode_t *node = alloc_astnode(AST_BINOP);
    node->binary.op = op;
    node->binary.left = left;
    node->binary.right = right;
    return node;
}

astnode_t *alloc_astnode_ident(string_t ident) {
    astnode_t *node = alloc_astnode(AST_IDENT);
    node->ident.str = ident;
    return node;
}

astnode_t *alloc_astnode_number(number_t number) {
    astnode_t *node = alloc_astnode(AST_NUM);
    node->num.num = number;
    return node;
}

astnode_t *alloc_astnode_charlit(char charlit) {
    astnode_t *node = alloc_astnode(AST_CHARLIT);
    node->charlit.char_literal = charlit;
    return node;
};

astnode_t *alloc_astnode_string(string_t string) {
    astnode_t *node = alloc_astnode(AST_STRING);
    node->str.str = string;
    return node;
}

void print_ast(astnode_t *node) {
    switch(node->type) {
        case AST_BINOP:
            break;
        case AST_CHARLIT:
            break;
        case AST_STRING:
            break;
        case AST_NUM:
            break;
        case AST_IDENT:
            fprintf(stdout, "IDENT: %s\n", node->str.str.string_literal);
            break;
        default:
            break;
    }
}

