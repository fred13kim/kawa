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

void print_symtable(symtable_t *table) {
    if (!table){
        yywarn("symbol table does not exist");
        return;
    }
    if (!table->list) {
        yywarn("symbol table is empty");
        return;
    }
    switch(table->scope) {
        case SCOPE_GLOBAL:  fprintf(stdout, "global scope\n"); break;
        case SCOPE_BLOCK:   fprintf(stdout, "block scope\n"); break;
        case SCOPE_FUNC:    fprintf(stdout, "func scope\n"); break;
        case SCOPE_PROTO:   fprintf(stdout, "proto scope\n"); break;
    }
    fprintf(stdout, "----\n");

    symtable_entry_list_t *cur_list = table->list;
    symtable_entry_node_t *cur_entry_node = cur_list->head;
    symtable_entry_t *entry;
    while (cur_entry_node) {
        entry = cur_entry_node->entry;
        switch(entry->attr_type) {
            case ATTR_VAR: 
                fprintf(stdout, "entry name: %s is declared at %s:%d\n", entry->name, entry->filename, entry->lineno);
                fprintf(stdout, "AST VAR DUMP:\n"); print_ast(entry->variable.type);
                fprintf(stdout, "\n");
                break;
            case ATTR_FUNC:
                fprintf(stdout, "entry name: %s is defined at %s:%d\n", entry->name, entry->filename, entry->lineno);
                fprintf(stdout, "AST FUNC DUMP:\n");
                print_ast(entry->function.type);
                fprintf(stdout, "BLOCK:\n");
                ;;
                print_ast(entry->function.type->func.block);
                fprintf(stdout, "SYMTABLE:\n");
                print_symtable(entry->function.type->func.block->compound_statement.table);
                fprintf(stdout, "\n");
                break;
        }
        cur_entry_node = cur_entry_node->next;
    }
}

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
        case AST_LL_LIST:
            fprintf(stdout, "LIST:\n");
            astnode_t *cur = astnode->ll_list.head;
            while(cur) {
                space++; print_ast(cur->ll_node.node); space--;
                cur = cur->ll_node.next;
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

        case AST_DECLARATION:
            fprintf(stdout, "DECLARATION:\n");
            space++; print_ast(astnode->declaration.init_declarator_list); space--;
            break;

        case AST_DECLARATION_SPEC:
            switch(astnode->declaration_spec.spec) {
                case STORAGE_TYPEDEF:   fprintf(stdout, "typedef "); break;
                case STORAGE_EXTERN:    fprintf(stdout, "extern "); break;
                case STORAGE_STATIC:    fprintf(stdout, "static "); break;
                case STORAGE_AUTO:      fprintf(stdout, "auto "); break;
                case STORAGE_REGISTER:  fprintf(stdout, "register "); break;
                case TYPE_VOID:         fprintf(stdout, "void "); break;
                case TYPE_CHAR:         fprintf(stdout, "char "); break;
                case TYPE_SHORT:        fprintf(stdout, "short "); break;
                case TYPE_INT:          fprintf(stdout, "int "); break;
                case TYPE_LONG:         fprintf(stdout, "long "); break;
                case TYPE_FLOAT:        fprintf(stdout, "float "); break;
                case TYPE_DOUBLE:       fprintf(stdout, "double "); break;
                case TYPE_SIGNED:       fprintf(stdout, "signed "); break;
                case TYPE_UNSIGNED:     fprintf(stdout, "unsigned "); break;
                case TYPE__BOOL:        fprintf(stdout, "bool "); break;
                case TYPE__COMPLEX:     fprintf(stdout, "complex "); break;
                case TYPE_CONST:        fprintf(stdout, "const "); break;
                case TYPE_RESTRICT:     fprintf(stdout, "restrict "); break;
                case TYPE_VOLATILE:     fprintf(stdout, "volatile "); break;
                case FUNC_INLINE:       fprintf(stdout, "inline "); break;
            }
            fprintf(stdout,"\n");
            break;

        case AST_PTR:
            fprintf(stdout, "PTR to\n");
            space++; print_ast(astnode->ptr.ptr_to); space--;
            break;
        case AST_ARRAY:
            fprintf(stdout, "ARRAY with size:\n");
            space++; print_ast(astnode->array.size); space--;
            print_indent(space); fprintf(stdout, "of\n");
            space++; print_ast(astnode->array.ptr_to); space--;
            break;
        case AST_FUNC:
            fprintf(stdout, "FUNC with ret type:\n");
            space++; print_ast(astnode->func.ret_type); space--;
            break;

        case AST_COMPOUND_STATEMENT:
            fprintf(stdout, "COMPOUND STATEMENT:\n");
            if (!astnode->compound_statement.block_items) {
                fprintf(stdout, "empty block\n");
                return;
            }
            astnode_t *block_items = astnode->compound_statement.block_items;
            space++; print_ast(block_items); space--;
            break;

        case AST_LABELED_STATEMENT:
            switch(astnode->labeled_statement.type) {
                case LABEL_ID:
                    fprintf(stdout, "LABEL:\n");
                    space++; print_ast(astnode->labeled_statement.ident); space--;
                    space++; print_ast(astnode->labeled_statement.statement); space--;
                    break;
                case LABEL_CASE:
                    fprintf(stdout, "CASE:\n");
                    space++; print_ast(astnode->labeled_statement.cond); space--;
                    space++; print_ast(astnode->labeled_statement.statement); space--;
                    break;
                case LABEL_DEF:
                    fprintf(stdout, "DEFAULT:\n");
                    space++; print_ast(astnode->labeled_statement.statement); space--;
                    break;
            }
            break;
            
        case AST_SELECT_STATEMENT:
            switch(astnode->select_statement.type) {
                case SELECT_IF:
                    fprintf(stdout, "IF:\n");
                    space++; print_ast(astnode->select_statement.cond); space--;
                    print_indent(space); fprintf(stdout, "THEN:\n");
                    space++; print_ast(astnode->select_statement.statement); space--;
                    break;
                case SELECT_ELIF:
                    fprintf(stdout, "IF:\n");
                    space++; print_ast(astnode->select_statement.cond); space--;
                    print_indent(space); fprintf(stdout, "THEN:\n");
                    space++; print_ast(astnode->select_statement.statement); space--;
                    print_indent(space); fprintf(stdout, "ELSE:\n");
                    space++; print_ast(astnode->select_statement.else_statement); space--;
                    break;
                case SELECT_SWITCH:
                    fprintf(stdout, "SWITCH:\n");
                    print_indent(space); fprintf(stdout, "COND:\n");
                    space++; print_ast(astnode->select_statement.cond); space--;
                    print_indent(space); fprintf(stdout, "STATEMENT:\n");
                    space++; print_ast(astnode->select_statement.statement); space--;
                    break;
            }
            break;

        case AST_ITERATION_STATEMENT:
            switch(astnode->iteration_statement.type) {
                case ITER_WHILE:
                    fprintf(stdout, "WHILE:\n");
                    space++; print_ast(astnode->iteration_statement.cond); space--;
                    print_indent(space); fprintf(stdout, "DO:\n");
                    space++; print_ast(astnode->iteration_statement.statement); space--;
                    break;
                case ITER_DO:
                    fprintf(stdout, "DO:\n");
                    space++; print_ast(astnode->iteration_statement.statement); space--;
                    print_indent(space); fprintf(stdout, "WHILE:\n");
                    space++; print_ast(astnode->iteration_statement.cond); space--;
                    break;
                case ITER_FOR:
                    fprintf(stdout, "FOR:\n");
                    print_indent(space); fprintf(stdout, "INIT:\n");
                    space++; print_ast(astnode->iteration_statement.clause1); space--;
                    print_indent(space); fprintf(stdout, "COND:\n");
                    space++; print_ast(astnode->iteration_statement.cond); space--;
                    print_indent(space); fprintf(stdout, "UPDATE:\n");
                    space++; print_ast(astnode->iteration_statement.expr3); space--;
                    print_indent(space); fprintf(stdout, "BODY:\n");
                    space++; print_ast(astnode->iteration_statement.statement); space--;
                    break;
            }
            break;

        case AST_JUMP_STATEMENT:
            switch(astnode->jump_statement.type) {
                case JUMP_GOTO:
                    fprintf(stdout, "GOTO:\n");
                    space++; print_ast(astnode->jump_statement.ident); space--;
                    break;
                case JUMP_CONTINUE:
                    fprintf(stdout, "CONTINUE\n");
                    break;
                case JUMP_BREAK:
                    fprintf(stdout, "BREAK\n");
                    break;
                case JUMP_RETURN:
                    fprintf(stdout, "RETURN:\n");
                    space++; print_ast(astnode->jump_statement.expr); space--;
                    break;
            }
            break;


        default:
            break;
    }
}
