#include "../include/typecheck.h"
#include "../include/scratch.h"
#include "../include/label.h"
void expr_codegen( struct expr *e );
void stmt_codegen( struct stmt *s );
int decl_codegen( struct decl *d );
const char *symbol_codegen( struct symbol *s );