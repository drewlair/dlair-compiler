#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_INCR,
	EXPR_DECR,
	EXPR_EE,
	EXPR_GE,
	EXPR_LT,
	EXPR_GT,
	EXPR_LE,
	EXPR_NE,
	EXPR_OR,
	EXPR_NOT,
	EXPR_NEGATE,
	EXPR_POS,
	EXPR_AND,
	EXPR_EXP,
	EXPR_ASSIGN,
	EXPR_INTEGER_LITERAL,
	EXPR_FLOAT_LITERAL,
	EXPR_BOOLEAN_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_STRING_LITERAL,
	EXPR_IDENT_LITERAL,
    EXPR_LIST,
	EXPR_LIST_ARRAY,
    EXPR_LIST_INIT,
	EXPR_INDEX_LIST,
	EXPR_INDEX,
	EXPR_PAREN,
	EXPR_CALL,
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	float float_literal;
	char char_literal;
	const char * string_literal;
	struct symbol *symbol;
};
/*
struct expr_list {
	struct expr *expr;
	struct expr_list *next;
};
*/
extern struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );
extern struct expr * expr_create_name( const char *n, expr_t kind);
extern struct expr * expr_create_integer_literal( int c );
extern struct expr * expr_create_boolean_literal( const char *str );
extern struct expr * expr_create_float_literal( float f );
extern struct expr * expr_create_char_literal( char c );
extern struct expr * expr_create_string_literal( const char *str );


extern void expr_print( struct expr *e );

#endif
