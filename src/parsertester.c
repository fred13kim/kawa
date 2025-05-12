#include "parser.h"
#include "parser_defs.h"
#include "symtable.h"


symtable_t *table;
int main(void)
{
    table = symtable_create(SCOPE_GLOBAL);
    yyparse();
}
