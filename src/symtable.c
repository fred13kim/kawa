#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
symtable_entry_t *lookup(symtable_t *table, symtable_entry_t *entry) {
    symtable_t *cur = table;
    symtable_entry_list_t *cur_list;
    symtable_entry_node_t *cur_node;

    while(cur) {
        /*
         * let's check through our entry list
         */
        cur_list = table->list;
        if (cur_list != NULL) {
            cur_node = cur_list->head;
            while(cur_node) {
                if (cur_node->entry != NULL) {
                    if (symtable_compare_entry(cur_node->entry, entry)) {
                        return cur_node->entry;
                    }
                }
                cur_node = cur_node->next;
            }
        }

        /*
         * if the entry is not in this scope go to the next scope
         */
        cur = cur->next;
    }
    return NULL;
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





