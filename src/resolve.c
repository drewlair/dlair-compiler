#include "../include/resolve.h"
#include "../include/main.h"


void decl_resolve( struct decl* d ){
    if (!d){
        return;
    }
    struct symbol *s;
   

    if ( stack->size == 1 ){
        s = symbol_create( SYMBOL_GLOBAL, d->type, d->name );
    }

    else{
        s = symbol_create( SYMBOL_LOCAL, d->type, d->name );
        s->which = stack->localCount++;
    }
    d->symbol = s;
    scope_bind( d->name, s );
    

    if (d->type->kind == TYPE_FUNCTION){
        if ( stack->size > 1 ){
            printf("resolve error: function declared in local scope\n");
        }
        
        scope_enter();
        param_list_resolve( d->type->params, 1 );
        stmt_resolve( d->code );
        scope_exit();
            
    }
    else{
        expr_resolve( d->value );
    }
    decl_resolve( d->next );
    

}

void param_list_resolve( struct param_list* param_list, int numParam ){
    if ( !param_list ){
        return;
    }
    struct symbol *s = symbol_create( SYMBOL_PARAM, param_list->type, param_list->name );
    s->which = numParam;
    scope_bind( param_list->name, s );
    param_list->symbol = s;
    param_list_resolve( param_list->next, numParam + 1 );
}

void stmt_resolve( struct stmt* s ){
    if ( !s ){
        return;
    }

    if( s->kind == STMT_DECL ){
        decl_resolve( s->decl );
    }

    else if ( s->kind == STMT_FOR ){
        scope_enter();
        expr_resolve( s->init_expr  );
        expr_resolve( s->expr       );
        expr_resolve( s->next_expr  );
        stmt_resolve( s->body       );
        scope_exit();
    }

    else if ( s->kind == STMT_IF_ELSE ){
        expr_resolve( s->init_expr );

        scope_enter();
        stmt_resolve( s->body );
        scope_exit();

        scope_enter();
        stmt_resolve( s->else_body );
        scope_exit();
    }

    else{
        expr_resolve( s->init_expr );
    }

    stmt_resolve( s->next );

}

void expr_resolve( struct expr* e ){

    if ( !e ){
        return;
    }

    if ( e->kind == EXPR_IDENT_LITERAL ){
        struct symbol *s = scope_lookup( e->name );
        
        if ( !s ){
            printf("resolve error: %s is not defined\n", e->name);
        }
        
        else if( s->kind == SYMBOL_GLOBAL ){
           printf("%s resolves to global %s\n", s->name, s->name); 
        }
        else if (s->kind == SYMBOL_LOCAL){
            printf("%s resolves to local %d\n", s->name, s->which);
        }
        else{
            printf("%s resolves to param %d\n",s->name,s->which);
        }

    }
    else{
        expr_resolve( e->left );
        expr_resolve( e->right );
    }

}
