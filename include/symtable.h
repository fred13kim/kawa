#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>

typedef struct symtable_entry_s symtable_entry_t;
typedef struct symtable_entry_node_s symtable_entry_node_t;
typedef struct symtable_entry_list_s symtable_entry_list_t;
typedef struct symtable_s symtable_t;


symtable_t *symtable_create(int scope);
void symtable_destroy(symtable_t *table);
symtable_entry_t *lookup(symtable_t *table, symtable_entry_t *entry);
bool enter(symtable_t *table, symtable_entry_t *entry);


struct attr_variable_s {

};

struct attr_function_s {

};

struct attr_struct_union_tag_s {

};

struct attr_enum_tag_s {

};

struct attr_enum_constant_s {

};

struct attr_statement_label_s {

};

struct attr_typedef_name_s {

};

struct attr_struct_union_member_s {

};






struct symtable_entry_s {
    char *filename;
    int lineno;

    char *name;
    int namespace;

    int attr_type;
    union {
        struct attr_variable_s              variable;
        struct attr_function_s              function;
        struct attr_struct_union_tag_s      struct_union_tag;
        struct attr_enum_tag_s              enum_tag;
        struct attr_enum_constant_s         enum_constant;
        struct attr_statement_label_s       statement_label;
        struct attr_typedef_name_s          typedef_name;
        struct attr_struct_union_member_s   struct_union_member;
    };
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
