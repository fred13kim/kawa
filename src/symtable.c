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

    if (init_declarator_list == NULL) {
        yywarn("useless type name in empty declaration?!");
        return;
    }

    /*
     * must assign pointers from the ast nodes in the decl list the final
     * pointer (ptr, array, func) should point to the declaration_spec_list
     * must also split the variables too
     * this is done by splitting the idents
     * note that the declarator_list can only have the following astnodes:
     * AST_IDENT
     * AST_PTR
     * AST_ARRAY
     * AST_FUNC
     */

    astnode_t *cur = init_declarator_list->ll_list.head;
    astnode_t *next;
    astnode_t *tmp;
    astnode_t *cur_node;
    astnode_t *tmp_node;
    astnode_t *list;
    symtable_entry_t *entry;

    // first node should always be an ident by grammar unless it is a function
    while (cur) {
        cur_node = cur->ll_node.node;
        // reduce logic
        if (cur_node->type == AST_IDENT) {
            tmp = alloc_astnode_ll_node(cur_node); // wrap the ident node in a
                                                   // ll_node
            list = alloc_astnode_ll_list(tmp);
            // first check if the next node exists & is not an ident
            next = cur->ll_node.next;
            while (next != NULL && next->ll_node.node->type != AST_IDENT) {
                // what follows should be all the declarators for this variable
                // if cur is a ptr or array, we want to have it point to the
                // next declarator
                switch (cur_node->type) {
                    case AST_PTR: 
                        cur_node->ptr.ptr_to = next->ll_node.node;
                        break;
                    case AST_ARRAY:
                        cur_node->array.ptr_to = next->ll_node.node;
                        break;
                }
                append_astnode(list, next->ll_node.node);
                cur = cur->ll_node.next;
                next = cur->ll_node.next;
                cur_node = cur->ll_node.node;
            }
        }

        // top level reduction... 
        switch(cur_node->type) {
            case AST_PTR:
                cur_node->ptr.ptr_to = declaration_spec_list;
                break;
            case AST_ARRAY:
                cur_node->array.ptr_to = declaration_spec_list;
                break;
        }
        append_astlist(list,declaration_spec_list);

        tmp_node = pop_head_astlist(list);
        entry = alloc_symtable_entry(tmp_node->ident.str.string_literal, NAMESPACE_ETC, ATTR_VAR);
        entry->variable.type = list->ll_list.head->ll_node.node;

        if (!symtable_enter(table,entry)) {
                yyerror("symbol entry already exists in the table");
                exit(-1);
        }


        // iterate to the next variable
        cur = cur->ll_node.next;
    }
}
void symtable_func_def() {

}





