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

astnode_t *alloc_astnode_declaration(astnode_t *declaration_specifier_list, astnode_t *init_declarator_list) {
    astnode_t *astnode = alloc_astnode(AST_DECLARATION);
    astnode->declaration.declaration_specifier_list = declaration_specifier_list;
    astnode->declaration.init_declarator_list = init_declarator_list;
    return astnode;
}

astnode_t *alloc_astnode_compound_statement() {
}





void print_indent(int num) {
    for(int i = 0; i < num; i++) {
        fprintf(stdout, " ");
    }
}

/*
 * DESCRIPTION:
 * The get_op() function returns the operator string from the provided
 * compound operator
 */
void print_op(int op) {
    if (op < 255) {
        fprintf(stdout, "%c", op);
        return;
    }
    char *s;
    switch(op) {
        case TIMESEQ:   s = "*";  break;
        case DIVEQ:     s = "/";  break;
        case MODEQ:     s = "%";  break;
        case PLUSEQ:    s = "+";  break;
        case MINUSEQ:   s = "-";  break;
        case SHLEQ:     s = "<<"; break;
        case SHREQ:     s = ">>"; break;
        case ANDEQ:     s = "&"; break;
        case OREQ:      s = "|";  break;
        case XOREQ:     s = "^";  break;

        case PLUSPLUS:      s = "++"; break;
        case MINUSMINUS:    s = "--"; break;
        case SHL:           s = "<<"; break;
        case SHR:           s = ">>"; break;
        case LTEQ:          s = "<="; break;
        case GTEQ:          s = ">="; break;
        case EQEQ:          s = "=="; break;
        case NOTEQ:         s = "!="; break;
        case LOGAND:        s = "&&"; break;
        case LOGOR:         s = "||"; break;
    }
    fprintf(stdout,"%s",s);
}

static int space = 2;

void print_ast(astnode_t *astnode) {
    print_indent(space);
    switch(astnode->type) {
        case AST_UNARY:
            switch(astnode->unary.op) {
                case '&':           fprintf(stdout, "ADDRESSOF");                      break;
                case '*':           fprintf(stdout, "DEREF");                          break;
                case PLUSPLUS:      fprintf(stdout, "UNARY OP POSTINC");               break;
                case MINUSMINUS:    fprintf(stdout, "UNARY OP POSTDEC");               break;
                default:            fprintf(stdout, "UNARY OP %c", astnode->unary.op); break;
            }
            fprintf(stdout,"\n");
            space++; print_ast(astnode->unary.expr); space--;
            break;
        case AST_BINARY:
            if(astnode->binary.op == '=') {
                fprintf(stdout, "ASSIGNMENT");
            }
            else if(astnode->binary.op >= TIMESEQ && astnode->binary.op <= XOREQ) {
                fprintf(stdout, "ASSIGNMENT COMPOUND (");
                print_op(astnode->binary.op);
                fprintf(stdout, ")");
            }
            else if(astnode->binary.op >= LTEQ && astnode->binary.op <= NOTEQ){
                fprintf(stdout, "COMPARISON OP ");
                print_op(astnode->binary.op);
            }
            else if(astnode->binary.op >= LOGAND && astnode->binary.op <= LOGOR){
                fprintf(stdout, "LOGICAL OP ");
                print_op(astnode->binary.op);
            }
            else {
                fprintf(stdout, "BINARY OP ");
                print_op(astnode->binary.op);
            }
            fprintf(stdout,"\n");
            space++;
            print_ast(astnode->binary.expr1);
            print_ast(astnode->binary.expr2);
            space--;
            break;
        case AST_TERNARY:
            fprintf(stdout, "TERNARY OP, IF:\n");
            space++; print_ast(astnode->ternary.expr1); space--;
            print_indent(space);
            fprintf(stdout, "THEN:\n");
            space++; print_ast(astnode->ternary.expr2); space--;
            print_indent(space);
            fprintf(stdout, "ELSE:\n");
            space++; print_ast(astnode->ternary.expr3); space--;
            break;
        case AST_SELECT:
            if((astnode->select.expr)->unary.op && ((astnode->select.expr)->unary.op == '*')) {
                fprintf(stdout, "INDIRECT SELECT, member %s\n", (astnode->select.ident)->ident.str.string_literal);
                space++; print_ast((astnode->select.expr)->unary.expr); space--;
            }
            else {
                fprintf(stdout, "SELECT, member %s\n", (astnode->select.ident)->ident.str.string_literal);
                space++; print_ast(astnode->select.expr); space--;
            }
            break;
        case AST_FNCALL:
            if(!(astnode->fncall.args)) {
                fprintf(stdout, "FNCALL\n");
                space++; print_ast(astnode->fncall.name); space--;
            }
            else {
                fprintf(stdout, "FNCALL, %d arguments\n", astnode->fncall.args->ll_list.size);
                space++; print_ast(astnode->fncall.name); space--;
                astnode_t *cur = astnode->fncall.args->ll_list.head;
                int count = 1;
                while(cur) {
                    print_indent(space);
                    fprintf(stdout, "arg #%d=\n", count++);
                    space++; print_ast(cur->ll_node.node); space--;
                    cur = cur->ll_node.next;
                }

            }
            break;
        case AST_SIZEOF:
            fprintf(stdout,"SIZEOF\n");
            space++; print_ast(astnode->_sizeof.expr); space--;
            break;
        case AST_IDENT:
            fprintf(stdout, "IDENT %s\n", astnode->ident.str.string_literal);
            break;
        case AST_NUM:
            if (astnode->num.num.type >= NUM_INT )
                fprintf(stdout, "CONSTANT: (type=int)%lld\n", astnode->num.num.integer);
            else
                fprintf(stdout, "CONSTANT: (type=double)%Lg\n", astnode->num.num.real);
            break;
        // case AST_CHARLIT:
        //     fprintf(stdout, "CHARLIT %o\n", astnode->charlit.char_literal);
        //     break;
        case AST_STRING:
            fprintf(stdout, "STRING\t%s\n", astnode->str.str.string_literal);
            break;
        default:
            break;
    }
}


