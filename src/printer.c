#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "ast.h"
#include "printer.h"


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


