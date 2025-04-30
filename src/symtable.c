#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

symtable_t *symtable_create(int scope) {
    symtable_t *table = malloc(sizeof(symtable_t));
    table->scope = scope;
    return table;
}

void symtable_destroy(symtable_t *table) {
    /* add destruction logic here */
    free(table);
}


/*
 * in our current scope, let's check if the entry is in the list
 */
symtable_entry_t *lookup(symtable_t *table, symtable_entry_t *entry) {
    symtable_t *cur = table;
    symtable_entry_list_t *cur_list;
    symtable_entry_t *tmp;
    while(cur) {

        cur = cur->next;
    }
}

bool enter(symtable_t *table, symtable_entry_t *entry) {
    /*
     * first check if the entry is in our table if not proceed
     */

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





