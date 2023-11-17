#include "../include/typecheck.h"

struct type* decl_typecheck( struct decl *d ){
    if (!d){
        return NULL;
    }
    struct type *t;
    
    
    if(d->symbol->type->kind == TYPE_FUNCTION){
       
        if (d->symbol->type->subtype->kind == TYPE_ARRAY || d->symbol->type->subtype->kind == TYPE_FUNCTION){
            printf("type error: function prototype %s returns invalid type ", d->name);
            type_print(d->symbol->type->subtype);
            printf("\n");
            typechecker_result = 0;
        }
        
        param_list_type_check(d->symbol->type->params);
        stmt_typecheck(d->code);
        struct returnTypes *curr = returnTypesHead;
        while (curr){
            if (!type_compare(curr->type, d->symbol->type->subtype)){
                printf("type error: return type of ");
                type_print(curr->type);
                printf(" of expression ");
                expr_print(curr->expr);
                printf(" from function %s of return type ",d->name);
                type_print(d->symbol->type->subtype);
                printf("\n");
                typechecker_result = 0;
            }
            curr = curr->next;
        }
        returnTypesDestructor(returnTypesHead);
        returnTypesHead = NULL;
        returnTypesTail = NULL;

    } else if(d->symbol->type->kind == TYPE_VOID){
        printf("type error: cannot make non-function declaration of %s type void\n", d->name);
        typechecker_result = 0;
    } 
    if (d->symbol->type->kind == TYPE_ARRAY){
        if (!check_array_type(d->symbol->type)){
            printf("type error: declared %s of type ", d->name);
            type_print(d->symbol->type);
            printf(" with elements of type void or function\n");
            typechecker_result = 0;
        }
        if (d->symbol->kind == SYMBOL_LOCAL){
            printf("type error: declared local variable %s with type ", d->name);
            type_print(d->symbol->type);
            printf(", cannot declare local variables with array typeeeee\n");
            typechecker_result = 0;
        }
        else if (d->symbol->kind == SYMBOL_GLOBAL){
            check_arr_global_size(d->symbol->type);
        }
        if (d->value){
            t = expr_typecheck(d->value);
            if (!type_compare(d->symbol->type, t)){
                printf("type error: attempted to %s of type ", d->name);
                type_print(d->symbol->type);
                printf(" with expression ");
                expr_print(d->value);
                printf(" of type ");
                type_print(t);
                printf("\n");
                typechecker_result = 0;
            }
        }
    } else if(d->value){
        t = expr_typecheck(d->value);
        if (!type_compare(d->symbol->type, t)){
            printf("type error: set expression ");
            expr_print(d->value);
            printf(" of type ");
            type_print(t);
            printf(" as value for the declaration of %s of type ", d->name);
            type_print(d->symbol->type);
            printf("\n");
            typechecker_result = 0;
        }
        if (d->value->kind == EXPR_IDENT_LITERAL && d->symbol->kind == SYMBOL_GLOBAL){
            printf("type error: set non-constant initializer %s of type ", d->value->symbol->name);
            type_print(t);
            printf(" to global variale %s\n", d->name);
            typechecker_result = 0;
        }

    }
    decl_typecheck(d->next);


    
    return NULL;
}

struct type* stmt_typecheck( struct stmt *s ){
    if (!s){
        return NULL;
    }

    struct type *t;

    switch(s->kind){
        case STMT_DECL:
            decl_typecheck(s->decl);
            break;
        case STMT_EXPR:
            expr_typecheck(s->init_expr);
            break;
        case STMT_RETURN:
            t = expr_typecheck(s->init_expr);
            if (!t){
                t = type_create(TYPE_VOID, NULL, NULL, NULL);
            }
            if (!returnTypesHead){
                returnTypesHead = returnTypesCreate(t, s->init_expr, NULL);
                returnTypesTail = returnTypesHead;
            }
            else{
                returnTypesTail->next = returnTypesCreate(t, s->init_expr, NULL);
                returnTypesTail = returnTypesTail->next;
            }
            break;
        case STMT_PRINT:
            expr_typecheck(s->init_expr);
            break;
        case STMT_IF_ELSE:
            t = expr_typecheck(s->init_expr);
            if (!t){
                printf("type error: no expression given for if statement\n");
                typechecker_result = 0;
            }
            else if (t->kind != TYPE_BOOLEAN){
                printf("type error: condition ");
                expr_print(s->init_expr);
                printf(" for if statement is of type ");
                type_print(t);
                printf(" and not of type boolean\n");
                typechecker_result = 0;
            }
            stmt_typecheck(s->body);
            stmt_typecheck(s->else_body);
            break;
        case STMT_FOR:
            expr_typecheck(s->init_expr);
            expr_typecheck(s->expr);
            expr_typecheck(s->next_expr);
            stmt_typecheck(s->body);
            break;
    }
    stmt_typecheck(s->next);
    
    return NULL;
}

struct type* expr_typecheck( struct expr *e ){
    if (!e){
        return NULL;
    }
    struct type *left;
    struct type *right;
    struct arr_result *r;

    switch(e->kind){
        case EXPR_IDENT_LITERAL:
            return e->symbol->type;
            break;
        case EXPR_INTEGER_LITERAL:
            return type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_FLOAT_LITERAL:
            return type_create(TYPE_FLOAT, NULL, NULL, NULL);
            break;
        case EXPR_BOOLEAN_LITERAL:
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_CHAR_LITERAL:
            return type_create(TYPE_CHARACTER, NULL, NULL, NULL);
            break;
        case EXPR_STRING_LITERAL:
            return type_create(TYPE_STRING, NULL, NULL, NULL);
            break;
        case EXPR_OR:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "||");
                typechecker_result = 0;
            }
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_AND:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "&&");
                typechecker_result = 0;
            }
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_EE:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "==");
                typechecker_result = 0;
            }
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_GE:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, ">=");
                typechecker_result = 0;
            }
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_LE:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "<=");
                typechecker_result = 0;
            }

            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_GT:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, ">");
                typechecker_result = 0;
            }
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL); 
            break;
        case EXPR_LT:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "<");
                typechecker_result = 0;
            }
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_NE:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "!=");
                typechecker_result = 0;
            }
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_ADD:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "+");
                typechecker_result = 0;
            }
            return left;
        case EXPR_SUB:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "-");
                typechecker_result = 0;
            }
            return left;
            break;
        case EXPR_MUL:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "*");
                typechecker_result = 0;
            }
            return left;
            break;
        case EXPR_DIV:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "/");
                typechecker_result = 0;
            }
            return left;
            break;
        case EXPR_MOD:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "%");
                typechecker_result = 0;
            }
            if (left->kind != TYPE_INTEGER){
                printf("type error: modulo operator with ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf(" and not integer\n");
                typechecker_result = 0;
            }
            if (right->kind != TYPE_INTEGER){
                printf("type error: modulo operator with ");
                expr_print(e->right);
                printf(" of type ");
                type_print(right);
                printf(" and not integer\n");
                typechecker_result = 0;
            }
            return type_create(TYPE_INTEGER, NULL, NULL, NULL);
            break;
        case EXPR_EXP:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (left->kind != TYPE_INTEGER && left->kind != TYPE_FLOAT){
                printf("type error: exponentiated ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf(" which is not an integer or float\n");
                typechecker_result = 0;
            }
            if (right->kind != TYPE_INTEGER && right->kind != TYPE_FLOAT){
                printf("type error: exponentiated ");
                expr_print(e->right);
                printf(" of type ");
                type_print(right);
                printf(" which is not an integer or float\n");
                typechecker_result = 0;
            }
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "^");
                typechecker_result = 0;
            }
            return type_create(TYPE_FLOAT, NULL, NULL, NULL);;
            break;
        case EXPR_ASSIGN:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (!type_compare(left, right)){
                error_print(e->left,e->right, "=");
                typechecker_result = 0;
            }
            return left;
            break;

        // pos, negate, and not trees are just continuous left nodes until a non-unary node is reached.
        // just need to recurse down to the leaf of the unaries
        case EXPR_NEGATE:
            left = expr_typecheck(e->left);
            if (left->kind != TYPE_FLOAT && left->kind != TYPE_INTEGER){
                printf("type error: cannot negate ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf("\n");
                typechecker_result = 0;
                return type_create(TYPE_INTEGER, NULL, NULL, NULL);
            }
            return left;
            break;
        case EXPR_POS:
            left = expr_typecheck(e->left);
            if (left->kind != TYPE_FLOAT && left->kind != TYPE_INTEGER){
                printf("type error: cannot negate ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf("\n");
                typechecker_result = 0;
                return type_create(TYPE_FLOAT, NULL, NULL, NULL);
            }
            
            return type_create(left->kind, NULL, NULL, NULL);
            
            
            break;
        case EXPR_NOT:
            left = expr_typecheck(e->left);
            if (left->kind != TYPE_BOOLEAN){
                error_print(e->left, NULL, "!");
                typechecker_result = 0;
            }
            return type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
            break;
        case EXPR_INCR:
            left = expr_typecheck(e->left);
            if (left->kind != TYPE_INTEGER && left->kind != TYPE_FLOAT){
                printf("type error: incremented expression ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf(" and not integer or float\n");
                typechecker_result = 0;
                return type_create(TYPE_INTEGER, NULL, NULL, NULL);
            }
            return left;
            break;
        case EXPR_DECR:
            left = expr_typecheck(e->left);
            if (left->kind != TYPE_INTEGER && left->kind != TYPE_FLOAT){
                printf("type error: decremented expression ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf(" and not integer or float\n");
                typechecker_result = 0;
                return type_create(TYPE_INTEGER, NULL, NULL, NULL);
            }
            return left;
            break;
        
        //lists are for arrays and follow the structure of right recursion for list nodes
        //until NULL is found with each value in the list being found on the left nodes
        //EXPR_LIST_ARRAY is the start of a new list and contains the EXPR_LIST node on its left pt
        case EXPR_LIST:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if ( left && e->left->kind == EXPR_IDENT_LITERAL){
                printf("type error: cannot initialize array with identifier ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf("\n");
                typechecker_result = 0;
            }
            if (!right){
                struct type* t = type_create(TYPE_ARRAY, type_copy(left), NULL, expr_create_integer_literal(1));
                return t;
            }
            else{
                if (!type_compare(left, right->subtype)){
                    printf("type error: non-equal expr types ");
                    type_print(left);
                    printf(" and ");
                    type_print(right);
                    printf(" in array initialization expression ");
                    expr_print(e->right);
                    printf("\n");
                    typechecker_result = 0;
                }
               
                return type_create(TYPE_ARRAY, type_copy(left), NULL, expr_create_integer_literal(right->arr_length->literal_value + 1));
            }
            break;
        case EXPR_LIST_ARRAY:
            left = expr_typecheck(e->left);
            return left;
            break;
            

        //In paren's, it's just a string a left paren nodes until a terminal expr is reached
        case EXPR_PAREN:
            return expr_typecheck(e->left);
            break;

        //EXPR_INDEX has the id of the arr on its left node and a string of EXPR_INDEX_LIST on its right node
        //EXPR_INDEX_LIST is the same structure as the EXPR_LIST_ARRAY
        case EXPR_INDEX_LIST:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);
            if (left->kind != TYPE_INTEGER){
                printf("type error: index ");
                expr_print(e->left);
                printf(" is of type ");
                type_print(left);
                printf(" and not integer\n");
                typechecker_result = 0;
            }
            if (!right) return type_create(TYPE_ARRAY, NULL, NULL, NULL);
            
            else return type_create(TYPE_ARRAY, right, NULL, NULL);
            break;
        case EXPR_INDEX:
            left = expr_typecheck(e->left);
            right = expr_typecheck(e->right);

            r = arr_compare(left, right);
            if (!r->failure){
                printf("type error: indexing of ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf(" assuming a ");
                type_print(right);
                typechecker_result = 0;
            }
            return r->type;
            
            break;
        case EXPR_CALL:
            left = expr_typecheck(e->left);
            if (left->kind != TYPE_FUNCTION){
                printf("type error: attempted function call with ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf("\n");
                typechecker_result = 0;
            }
            else if( !param_list_compare(left->params, e->right)){
                printf("type error: params not correct type or not correct amount for ");
                expr_print(e->left);
                printf(" of type ");
                type_print(left);
                printf(" with params ");
                param_list_print(left->params);
                printf("\n");
                typechecker_result = 0;
            }
            return left->subtype;
            break;
        
        case EXPR_PRINT_LIST:
            expr_typecheck(e->left);
            expr_typecheck(e->right);
            break;
        
    }
    //should happend for print statements
    return NULL;

}
int type_compare( struct type *left, struct type *right ){
    if (!left && !right){
        return 1;
    }
    else if (!left || !right){
        return 0;
    }
    
    if (left->kind != right->kind){
        return 0;
    }
    else if (left->kind == TYPE_ARRAY){
        if (right->kind == TYPE_ARRAY && left->arr_length && left->arr_length->literal_value == right->arr_length->literal_value)
            return type_compare(left->subtype, right->subtype); // the return type of the array and the right side must match
        else if(right->kind == TYPE_ARRAY && !left->arr_length){
            return type_compare(left->subtype, right->subtype);
        }
        else return 0;
    }

    else if (left->kind == TYPE_FUNCTION){
        return type_compare(left->subtype, right->subtype) && func_compare_param_list(left->params, right->params); // need to fix param_list_compare, same here, param_list_eq doesn't exist yet
    }
    // else return true
    return 1;

}

int func_compare_param_list(struct param_list *left, struct param_list *right){
    if (!left && !right) return 1;
    if (!left || !right) return 0;

    if (type_compare(left->type, right->type)) return func_compare_param_list(left->next, right->next);
    return 0;
}

struct param_list *create_param_list_types(struct expr *e){
    return param_list_create(NULL, expr_typecheck(e->left), create_param_list_types(e->right));
}

int param_list_compare(struct param_list *p, struct expr* e){
    if (!p && !e) return 1;
    if (!p || !e) return 0;

    if ( type_compare(p->type, expr_typecheck(e->left)) ){
        return param_list_compare(p->next, e->right);
    }
    else{
        return 0;
    }
}
// need to make it so both id and index are recrused until the correct type is returned.
struct arr_result *arr_compare(struct type* id, struct type* list){

    if (id->subtype->kind != TYPE_ARRAY && list->subtype){
        return arr_result_create(id->subtype, 0);
    }
    else if(!list->subtype){
       return arr_result_create(id->subtype, 1);
    }
    else{
        return arr_compare(id->subtype, list->subtype);
    }

    

}

struct arr_result *arr_result_create(struct type *t, int failure){
    struct arr_result *r = malloc(sizeof(struct arr_result));
    r->type = type_copy(t);
    r->failure = failure;
    return r;
}

struct type *type_copy(struct type *type){
    if (!type){
        return NULL;
    }
    if (type->kind == TYPE_ARRAY)
        return type_create(type->kind, type_copy(type->subtype), type->params, expr_create_integer_literal(type->arr_length->literal_value));
    else
        return type_create(type->kind, type_copy(type->subtype), type->params, NULL);
  

}

void error_print(struct expr* left, struct expr *right, char *op){
    printf("type error: cannot perform %s operation between a ", op);
    type_print(expr_typecheck(left));
    printf(" (");
    expr_print(left);
    printf(") and a ");
    type_print(expr_typecheck(right));
    printf(" (");
    expr_print(right);
    printf(")\n");
}

struct returnTypes *returnTypesCreate( struct type* type, struct expr *expr, struct returnTypes *next ){
    struct returnTypes *r = malloc( sizeof(struct returnTypes) );
    r->type = type;
    r->expr = expr;
    r->next = next;
    return r;
}

void returnTypesDestructor( struct returnTypes *r ){
    if (!r){
        return;
    }

    returnTypesDestructor( r->next );
    free(r);
    return;
}

int check_array_type(struct type *type){
    if (type->subtype->kind == TYPE_ARRAY){
        return check_array_type(type->subtype);
    } else if(type->subtype->kind == TYPE_VOID || type->subtype->kind == TYPE_FUNCTION){
        return 0;
    } else{
        return 1;
    }
}


void param_list_type_check( struct param_list *params ){
    
    if (!params){
        return;
    }
    struct type *t = params->type;
    if (t->kind == TYPE_ARRAY){
        if (!check_array_type(t)){
            printf("type error: declared param %s of type ", params->name);
            type_print(t);
            printf(" with elements of type void or function\n");
            typechecker_result = 0;
        }
    }
    else if (t->kind == TYPE_FUNCTION){
        printf("type error: declared param %s of type ", params->symbol->name);
        type_print(t);
        printf(" with elements of type function\n");
        typechecker_result = 0;
    }
    param_list_type_check( params->next );
    
}

void check_arr_global_size( struct type *type ){
    if (!type){
        return;
    }
    return;
    struct type* t = expr_typecheck(type->arr_length);
    
    if (!t){
        printf("type error: declared global array with non-integer size ");
        expr_print(type->arr_length);
        
        printf("\n");
        typechecker_result = 0;
    }
    printf("checking type ");
    type_print(t);
    check_arr_global_size( type->subtype );
    
}
