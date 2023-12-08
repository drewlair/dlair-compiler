#include "../include/decl.h"

struct decl* decl_create(const char* name, struct type* type, struct expr* value, struct stmt* code, struct decl* next ){
    struct decl *d = malloc( sizeof(struct decl) );

    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
    
    return d;
}

void decl_print( struct decl *d, int indent ){
    if (!d){
        return;
    }

    for (int i = 0; i < indent; i++){
        printf("\t");
    }

    type_print( d->type );
    if( d->code ){
        printf(" = \n{\n");
        stmt_print(d->code, indent+1);
        printf("}\n");

    }
    else if (d->value){
        printf(" = ");
        expr_print(d->value);
        printf(";\n");
    }

    else{
        printf(";\n");
    }
    decl_print(d->next, indent);


}
