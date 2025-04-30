#include <stdlib.h>
#include "symtable.h"


symtable_t *symtable_create(int scope) {
    symtable_t *table = malloc(sizeof(symtable_t));
    table->scope = scope;
    return table;
}

void destroy() {

}

void lookup() {

}

void enter() {

}
