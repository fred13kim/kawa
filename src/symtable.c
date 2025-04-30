#include <stdlib.h>
#include "symtable.h"


symtable_t *symtable_create(int scope) {
    symtable_t *table = malloc(sizeof(symtable_t));
    table->scope = scope;
    return table;
}

void symtable_destroy(symtable_t *table) {

}

symtable_entry_t *lookup(symtable_t *table, symtable_entry_t *entry) {

}

bool *enter(symtable_t *table, symtable_entry_t *entry) {

}
