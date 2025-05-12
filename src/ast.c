#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "ast.h"

astnode_t *alloc_astnode(enum nodetype t) {
    astnode_t *astnode = malloc(sizeof(astnode_t));
    if (!astnode) {
        fprintf(stderr,"Error: failed to malloc astnode\n");
        exit(-1);
    }
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

astnode_t *alloc_astnode_fncall(astnode_t *name, astnode_t *args) {
    astnode_t *astnode = alloc_astnode(AST_FNCALL);
    astnode->fncall.name = name;
    astnode->fncall.args = NULL;

    if (args) {
        astnode->fncall.args = args;
    }
    return astnode;
}

astnode_t *alloc_astnode_ll_list(astnode_t *head) {
    astnode_t *astnode = alloc_astnode(AST_LL_LIST);
    astnode->ll_list.head = head;
    astnode->ll_list.tail = head;
    astnode->ll_list.size = 1;
    return astnode;
}

astnode_t *alloc_astnode_ll_node(astnode_t *node) {
    astnode_t *astnode = alloc_astnode(AST_LL_NODE);
    astnode->ll_node.node = node;
    astnode->ll_node.next = NULL;
    return astnode;
}

/* create a ll node for the astnode and append to the list */
astnode_t *append_astnode(astnode_t *list, astnode_t *node) {
    astnode_t *astnode = alloc_astnode_ll_node(node);
    if (!(list->ll_list.tail)) {
        list->ll_list.head = astnode;
    }
    else {
        (list->ll_list.tail)->ll_node.next = astnode;
    }
    list->ll_list.tail = astnode;
    list->ll_list.size++;

    return list;
};


/* append a ll node to the list */
astnode_t *append_astnode_ll_node(astnode_t *list, astnode_t *ll_node) {
    if (!(list->ll_list.tail)) {
        list->ll_list.head = ll_node;
    }
    else {
        (list->ll_list.tail)->ll_node.next = ll_node;
    }
    list->ll_list.tail = ll_node;
    list->ll_list.size++;
    
    return list;
}

/* create a ll node for the astnode and prepend to the list */
astnode_t *prepend_astnode(astnode_t *list, astnode_t *node) {
    astnode_t *astnode = alloc_astnode_ll_node(node);
    if (!(list->ll_list.head)) {
        list->ll_list.tail = astnode;
    }
    else {
        astnode->ll_node.next = list->ll_list.head;
    }
    list->ll_list.head = astnode;
    list->ll_list.size++;

    return list;
}

/* prepend a ll node to the list */
astnode_t *prepend_astnode_ll_node(astnode_t *list, astnode_t *ll_node) {
    if (!(list->ll_list.head)) {
        list->ll_list.tail = ll_node;
    }
    else {
        ll_node->ll_node.next = list->ll_list.head;
    }
    list->ll_list.head = ll_node;
    list->ll_list.size++;

    return list;
}

/* append list2 to a list1 */
astnode_t *append_astlist(astnode_t *list1, astnode_t *list2) {
    (list1->ll_list.tail)->ll_node.next = list2->ll_list.head;
    list1->ll_list.tail = list2->ll_list.tail;

    list1->ll_list.size += list2->ll_list.size;
    return list1;
}

astnode_t *alloc_astnode_sizeof(astnode_t *expr) {
    astnode_t *astnode = alloc_astnode(AST_SIZEOF);
    astnode->_sizeof.expr = expr;
    return astnode;
};

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

// astnode_t *alloc_astnode_charlit(char charlit) {
//     astnode_t *astnode = alloc_astnode(AST_CHARLIT);
//     astnode->charlit.char_literal = charlit;
//     return astnode;
// };

astnode_t *alloc_astnode_string(string_t string) {
    astnode_t *astnode = alloc_astnode(AST_STRING);
    astnode->str.str = string;
    return astnode;
}

astnode_t *alloc_astnode_declaration(astnode_t *declaration_spec_list, astnode_t *init_declarator_list) {
    astnode_t *astnode = alloc_astnode(AST_DECLARATION);
    astnode->declaration.declaration_spec_list = declaration_spec_list;
    astnode->declaration.init_declarator_list = init_declarator_list;
    return astnode;
}

astnode_t *alloc_astnode_declaration_spec(int spec, int astnode_type) {
    astnode_t *astnode = alloc_astnode(AST_DECLARATION_SPEC);
    astnode->declaration_spec.spec = spec;
    return astnode;
}

astnode_t *alloc_astnode_ptr(void) {
    astnode_t *astnode = alloc_astnode(AST_PTR);
    astnode->ptr.type = NULL; // keep the ptr type to NULL for now
    return astnode;
}

astnode_t *alloc_astnode_array(astnode_t *expr) {
    astnode_t *astnode = alloc_astnode(AST_ARRAY);
    astnode->array.type = NULL;
    astnode->array.size = expr;
    return astnode;
}






