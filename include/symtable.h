#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include "ast.h"

typedef struct symtable_entry_s symtable_entry_t;
typedef struct symtable_entry_node_s symtable_entry_node_t;
typedef struct symtable_entry_list_s symtable_entry_list_t;
typedef struct symtable_s symtable_t;

enum scopes_enum {
    SCOPE_GLOBAL = 0,
    SCOPE_BLOCK,
    SCOPE_FUNC,
    SCOPE_PROTO,
};

enum entry_attr_type_enum {
    ATTR_VAR,
    ATTR_FUNC,
};

enum entry_namespace_enum {
    NAMESPACE_NULL,
    NAMESPACE_LABEL,
    NAMESPACE_TAG,
    NAMESPACE_SU_DEF,
    NAMESPACE_ETC, /* For all other identifier classes */
};

enum specifier_enum {
    STORAGE_TYPEDEF,
    STORAGE_EXTERN,
    STORAGE_STATIC,
    STORAGE_AUTO,
    STORAGE_REGISTER,
    TYPE_VOID,
    TYPE_CHAR,
    TYPE_SHORT,
    TYPE_INT,
    TYPE_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_SIGNED,
    TYPE_UNSIGNED,
    TYPE__BOOL,
    TYPE__COMPLEX,
    TYPE_CONST,
    TYPE_RESTRICT,
    TYPE_VOLATILE,
    FUNC_INLINE
};



symtable_t *symtable_create(int scope);
void symtable_destroy(symtable_t *table);
symtable_entry_t *symtable_lookup(symtable_t *table, symtable_entry_t *entry);
bool symtable_enter(symtable_t *table, symtable_entry_t *entry);

void symtable_start_declaration(astnode_t *declaration, symtable_t *table);
void symtable_func_def();

struct attr_variable_s {
    astnode_t *type;
    int offset; /* within stack frame for automatic storage class only ? */
};

struct attr_function_s {
    astnode_t *type;
};

struct attr_statement_label_s {

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
        struct attr_statement_label_s       statement_label_s;
        /* NOT SUPPORTING THESE T_T
        struct attr_struct_union_tag_s      struct_union_tag;
        struct attr_enum_tag_s              enum_tag;
        struct attr_enum_constant_s         enum_constant;
        struct attr_typedef_name_s          typedef_name;
        struct attr_struct_union_member_s   struct_union_member;
        */
    };
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
