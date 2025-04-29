#ifndef SYMTABLE_H
#define SYMTABLE_H

typedef struct symtable_s {

} symtable_t;

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

}

#endif // !SYMTABLE_H
