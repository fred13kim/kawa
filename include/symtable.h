#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef struct symtable_entry_s symtable_entry_t;
typedef struct symtable_entry_node_s symtable_entry_node_t;
typedef struct symtable_entry_list_s symtable_entry_list_t;
typedef struct symtable_s symtable_t;


symtable_t *symtable_create(int scope);
void symtable_destroy(symtable_t *table);
symtable_entry_t *lookup(symtable_t *table, symtable_entry_t *entry);
bool *enter(symtable_t *table, symtable_entry_t *entry);

struct symtable_entry_s {
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

};

struct symtable_entry_node_s {
    symtable_entry_t *entry;
    struct symtable_entry_node_s *next;
};

struct symtable_entry_list_s {
    int size;
    symtable_entry_node_t *head;
    symtable_entry_node_t *tail;
};

struct symtable_s {
    /* data within a symtable */
    int scope;
    symtable_entry_list_t *list;

    /* symtable points to another symtable */
    struct symtable_s *next;
};

#endif // !SYMTABLE_H
