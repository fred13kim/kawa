#include <stdlib.h>
#include "ast.h"

astnode_t *alloc_astnode(enum nodetype t) {
    astnode_t *node = malloc(sizeof(astnode_t));
    node->type = t;
    return node;
}

astnode_t *alloc_astnode_binary(int op, astnode_t *left, astnode_t *right) {
    astnode_t *node = alloc_astnode(AST_BINOP);
    node->binary.op = op;
    node->binary.left = left;
    node->binary.right = right;
    return node;
}
