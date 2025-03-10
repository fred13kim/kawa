#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

astnode_t *alloc_astnode(enum nodetype t) {
    astnode_t *node = malloc(sizeof(astnode_t));
    node->type = t;
    return node;
}

astnode_t *alloc_astnode_unary(int op, astnode_t *operand) {
    astnode_t *node = alloc_astnode(AST_UNARY);
    node->unary.op = op;
    node->unary.operand = operand;
    return node;
}

astnode_t *alloc_astnode_binary(int op, astnode_t *left, astnode_t *right) {
    astnode_t *node = alloc_astnode(AST_BINARY);
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

