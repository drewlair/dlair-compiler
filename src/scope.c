#include "../include/typecheck.h" // was typecheck.h
#include "../include/hash_table.h"



void scope_enter(){
    struct stack_node *newStack = malloc(sizeof(struct stack_node));

    newStack->ht = hash_table_create(0,0);

    if (!stack){
        stack = newStack;
        stack->prev = NULL;
        stack->size = 1;
    }
    else{
        newStack->prev = stack;
        newStack->size = stack->size + 1;
        stack = newStack;
    }
    
}


void scope_exit(){

    if (!stack){
        return;
    }
    struct stack_node *del = stack;
    stack = stack->prev;
    hash_table_delete(del->ht);
    free(del);

}



void scope_bind( const char *name, struct symbol* s ){
    struct symbol *tableSymbol;
    if (hash_table_insert( stack->ht, name, s ) != 1){
        tableSymbol = hash_table_lookup( stack->ht, name);
        if (s->type->kind != TYPE_FUNCTION){
            printf("resolve error: redeclaring %s of type ", tableSymbol->name);
            type_print(tableSymbol->type);
            printf(" with %s of type ", name);
            type_print(s->type);
            printf("\n");
            resolver_result = 1;
        }
        else if (s->type->kind == TYPE_FUNCTION && (!type_compare(tableSymbol->type, s->type) || !func_compare_param_list(tableSymbol->type->params, s->type->params))){
            printf("resolve error: redeclaring %s of type ", name);
            type_print(tableSymbol->type);
            printf("with params ");
            param_list_print(tableSymbol->type->params);
            printf(" to type ");
            type_print(s->type);
            printf(" with params ");
            param_list_print(s->type->params);
            printf("\n");
            resolver_result = 1;
        }
    }

    hash_table_remove( stack->ht, name );
    hash_table_insert( stack->ht, name, s);
        
    
    return;

}





struct symbol *scope_lookup( const char *name ){
    struct stack_node *stackPtr = stack;
    struct symbol *s = hash_table_lookup(stackPtr->ht, name);
    if (s) return s;
    stackPtr = stackPtr->prev;
    while (stackPtr && !s){
        s = hash_table_lookup(stackPtr->ht, name);
        stackPtr = stackPtr->prev;
    }
    return s;
}


