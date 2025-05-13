#ifndef PRINTER_H
#define PRINTER_H

#include "parser_defs.h"
#include "ast.h"

extern void yyerror(char const *);
extern void yywarn(char const *);
extern void yydebug(char const *);

void print_symtable(symtable_t *table);
void print_ast(astnode_t *node);

#endif // !PRINTER_H
