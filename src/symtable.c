#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#include "printer.h"

extern file_info_t file_info;
extern void yyerror(char const *);

symtable_entry_t *alloc_symtable_entry(char *name, int namespace, int type) {
    symtable_entry_t *entry = malloc(sizeof(symtable_entry_t));
    entry->name = name;
    entry->namespace = namespace;
    entry->attr_type = type;

    entry->lineno = file_info.lineno;
    entry->filename = strdup(file_info.filename);
    return entry;
}

symtable_t *symtable_create(int scope) {
    symtable_t *table = malloc(sizeof(symtable_t));
    table->scope = scope;
    return table;
}

void symtable_destroy(symtable_t *table) {
    /* add destruction logic here */
    free(table);
}

bool symtable_compare_entry(symtable_entry_t *entry1, symtable_entry_t *entry2) {
    if ((strcmp(entry1->name,entry2->name) == 0)
            && (entry1->namespace == entry2->namespace)) {
        return true;
    }
    return false;
}



/*
 * in our current scope, let's check if the entry is in the list
 */
symtable_entry_t *symtable_lookup(symtable_t *table, symtable_entry_t *entry) {
    symtable_t *cur = table;
    symtable_entry_list_t *cur_list;
    symtable_entry_node_t *cur_node;

    while(cur) {
        /*
         * let's check through our entry list
         */
        cur_list = table->list;
        if (cur_list == NULL) {
            cur = cur->next;
            continue;
        }

        cur_node = cur_list->head;
        while(cur_node) {
            if (cur_node->entry == NULL) {
                cur_node = cur_node->next;
                continue;
            }
            if (symtable_compare_entry(cur_node->entry, entry)) {
                return cur_node->entry;
            }
            cur_node = cur_node->next;
        }
        cur = cur->next;
    }
    return NULL;
}

bool symtable_enter(symtable_t *table, symtable_entry_t *entry) {
    /*
     * first check if the entry is in our table if not proceed
     */

    if (symtable_lookup(table, entry)) {
        return false;
    }

    symtable_entry_node_t *entry_node = malloc(sizeof(symtable_entry_node_t));
    if(!entry_node) {
        // error report here
    }
    entry_node->entry = entry;

    if (!table->list) {
        table->list = malloc(sizeof(symtable_entry_list_t));
        if (!table->list) {
            // error report here
        }
        table->list->head = entry_node;
        table->list->tail = entry_node;
        return true;
    }

    table->list->tail->next = entry_node;
    table->list->tail = entry_node;

    return true;
}

void symtable_start_declaration(astnode_t *declaration, symtable_t *table) {
    astnode_t *declaration_spec_list = declaration->declaration.declaration_spec_list;
    astnode_t *init_declarator_list = declaration->declaration.init_declarator_list;
    // print_ast(declaration);
    
    astnode_t *cur = init_declarator_list->ll_list.head;
    astnode_t *cur_node;
    astnode_t *parse_list;
    while(cur != NULL) {
        cur_node = cur->ll_node.node;

        if (cur_node->type == AST_IDENT) {
            if (cur->ll_node.next != NULL) {
                parse_list = reduce_astlist(cur);
                append_astlist(parse_list, declaration_spec_list);
            }
            else {
                parse_list = declaration_spec_list;
            }
            symtable_entry_t *entry = alloc_symtable_entry(cur_node->ident.str.string_literal, NAMESPACE_ETC, ATTR_VAR);
            entry->variable.type = parse_list->ll_list.head;
            if(!symtable_enter(table, entry)) {
                yyerror("RUH ROH SYMBOL ENTRY ALREADY EXISTS IN THE TABLE!!");
                exit(-1);
            }
        }
        cur = cur->ll_node.next;
    }
}
void symtable_func_def() {

}





