
#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include <stdio.h>
#include <string.h>

struct decl {
	const char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
	int numLocals;
};

struct decl * decl_create( const char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
void decl_print( struct decl *d, int indent );

#endif
