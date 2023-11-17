#include "main.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "decl.h"

struct arr_result{
    struct type *type;
    int failure;
};

struct type* decl_typecheck( struct decl *d );
struct type* stmt_typecheck( struct stmt *s );
struct type* expr_typecheck( struct expr *e );
int type_compare( struct type *left, struct type *right);
struct type *type_copy( struct type *type );
int param_list_compare( struct param_list *p, struct expr *e);
int func_compare_param_list(struct param_list *left, struct param_list *right);
struct arr_result *arr_compare( struct type* left, struct type* right );
struct arr_result *arr_result_create( struct type* type, int failure );
void error_print( struct expr *left, struct expr *right, char *op );
struct returnTypes *returnTypesCreate( struct type *type, struct expr* expr, struct returnTypes *next);
void returnTypesDestructor( struct returnTypes *r );
int check_array_type(struct type *type);
void param_list_type_check(struct param_list *params);
void check_arr_global_size(struct type *type);

int typechecker_result;