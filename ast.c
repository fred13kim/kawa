#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

astnode_t *alloc_astnode(enum nodetype t) {
    astnode_t *astnode = malloc(sizeof(astnode_t));
    astnode->type = t;
    return astnode;
}

astnode_t *alloc_astnode_unary(int op, astnode_t *expr) {
    astnode_t *astnode = alloc_astnode(AST_UNARY);
    astnode->unary.op = op;
    astnode->unary.expr = expr;
    return astnode;
}

astnode_t *alloc_astnode_binary(int op, astnode_t *expr1, astnode_t *expr2) {
    astnode_t *astnode = alloc_astnode(AST_BINARY);
    astnode->binary.op = op;
    astnode->binary.expr1 = expr1;
    astnode->binary.expr2 = expr2;
    return astnode;
}

astnode_t *alloc_astnode_ternary(astnode_t *expr1, astnode_t *expr2, astnode_t *expr3) {
    astnode_t *astnode = alloc_astnode(AST_TERNARY);
    astnode->ternary.expr1 = expr1;
    astnode->ternary.expr2 = expr2;
    astnode->ternary.expr3 = expr3;
    return astnode;
}

astnode_t *alloc_astnode_select(astnode_t *expr, string_t ident) {
    astnode_t *astnode_ident = alloc_astnode(AST_IDENT);
    astnode_ident->ident.str = ident;

    astnode_t *astnode = alloc_astnode(AST_SELECT);
    astnode->select.expr = expr;
    astnode->select.ident = astnode_ident;
    return astnode;
}

astnode_t *alloc_astnode_ident(string_t ident) {
    astnode_t *astnode = alloc_astnode(AST_IDENT);
    astnode->ident.str = ident;
    return astnode;
}

astnode_t *alloc_astnode_number(number_t number) {
    astnode_t *astnode = alloc_astnode(AST_NUM);
    astnode->num.num = number;
    return astnode;
}

astnode_t *alloc_astnode_charlit(char charlit) {
    astnode_t *astnode = alloc_astnode(AST_CHARLIT);
    astnode->charlit.char_literal = charlit;
    return astnode;
};

astnode_t *alloc_astnode_string(string_t string) {
    astnode_t *astnode = alloc_astnode(AST_STRING);
    astnode->str.str = string;
    return astnode;
}

void print_ast(astnode_t *node) {
    switch(node->type) {
        case AST_BINARY:
            break;
        case AST_IDENT:
            fprintf(stdout, "IDENT: %s\n", node->ident.str.string_literal);
            break;
        case AST_NUM:
            if (node->num.num.type >= NUM_INT )
                fprintf(stdout, "CONSTANT: (type=int) %lld\n", node->num.num.integer);
            else
                fprintf(stdout, "CONSTANT: (type=double) %Lg\n", node->num.num.real);
            break;
        case AST_CHARLIT:
            fprintf(stdout, "CHARLIT: %o\n", node->charlit.char_literal);
            break;
        case AST_STRING:
            fprintf(stdout, "STRING: %s\n", node->str.str.string_literal);
            break;
        default:
            break;
    }
}

