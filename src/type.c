#include "../include/type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params, struct expr* arr_length ){
    struct type *t = (struct type*) malloc( sizeof(struct type) );

    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
    t->arr_length = arr_length;

    return t;
}

void type_print( struct type *t ){
    if (!t){
        return;
    }
    else if (t->kind == TYPE_ARRAY){
        printf("array [");
        expr_print(t->arr_length);
        printf("] ");
        type_print(t->subtype);
    }
    else if (t->kind == TYPE_FUNCTION){
        printf("function ");
        type_print(t->subtype);
        printf(" ( ");
        param_list_print(t->params);
        printf(" ) ");
    }
    else{
        switch (t->kind){
            case TYPE_INTEGER:
                printf("integer");
                break;
            case TYPE_FLOAT:
                printf("float");
                break;
            case TYPE_AUTO:
                printf("auto");
                break;
            case TYPE_VOID:
                printf("void");
                break;
            case TYPE_BOOLEAN:
                printf("boolean");
                break;
            case TYPE_CHARACTER:
                printf("char");
                break;
            case TYPE_STRING:
                printf("string");
                break;
            default:
                printf("type error: %d",t->kind);
                break;

        }
    }


}
