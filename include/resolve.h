#include "../include/scope.h"
#include "../include/decl.h"
#include "../include/stmt.h"
#include "../include/expr.h"
#include "../include/param_list.h"

void decl_resolve( struct decl *d );
void param_list_resolve( struct param_list *param_list, int num );
void stmt_resolve( struct stmt *s );
void expr_resolve( struct expr *e );

