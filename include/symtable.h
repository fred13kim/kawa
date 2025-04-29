#ifndef SYMTABLE_H
#define SYMTABLE_H


typedef struct symtable_entry_s {
    char *filename;
    int lineno;

    /* will contain the following attritubtes:
     * variable
     * function
     * struct/union tag
     * enum tag
     * enum constant
     * statement label
     * typedef name
     * struct/union member
     */

} symtable_entry_t;

typedef struct symtable_ll_node_s {
    symtable_entry_t *entry;
    struct symtable_ll_node_s *next;
} symtable_ll_node_t;

typedef struct symtable_ll_list_s {
    int size;
    symtable_ll_node_t *head;
    symtable_ll_node_t *tail;
} symtable_ll;

#endif // !SYMTABLE_H
