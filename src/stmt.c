//dlair most recent version of stmt.c

#include "../include/stmt.h"

extern struct stmt* stmt_create(stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr,
    struct stmt *body, struct stmt *else_body, struct stmt *next ){
        struct stmt *s = malloc(sizeof(struct stmt));

        s->kind = kind;
        s->decl = decl;
        s->init_expr = init_expr;
        s->expr = expr;
        s->next_expr = next_expr;
        s->body = body;
        s->else_body = else_body;
        s->next = next;
        s->is_braced = false;

        return s;

}

void stmt_print( struct stmt* s, int indent ){
    if (!s){
        return;
    }


    switch (s->kind){

        case STMT_PRINT:
            for (int i = 0; i < indent; i++){
                printf("\t");
            }
            printf("print");
            if (s->init_expr){
                printf(" ");
                expr_print(s->init_expr);
            }
            printf(";");
            break;
        case STMT_RETURN:
            for (int i = 0; i < indent; i++){
                printf("\t");
            }
            printf("return ");
            expr_print(s->init_expr);
            printf(";");
            break;
        case STMT_FOR:
            for (int i = 0; i < indent; i++){
                printf("\t");
            }
            printf("for ( ");
            expr_print(s->init_expr);
            printf("; ");
            expr_print(s->expr);
            printf("; ");
            expr_print(s->next_expr);
            printf(" ) ");

            if (s->is_braced){
                printf("{\n");
                stmt_print(s->body, indent+1);
                for (int i = 0; i < indent; i++){
                    printf("\t");
                }
                printf("}");
            }
            else{
                printf("\n");
                stmt_print(s->body, indent + 1);
            }
            break;
        case STMT_EXPR:
            for (int i = 0; i < indent; i++){
                printf("\t");
            }
            expr_print(s->init_expr);
            printf(";");
            break;
        case STMT_IF_ELSE:
            for (int i = 0; i < indent; i++){
                printf("\t");
            }
            printf("if (");
            expr_print(s->init_expr);
            printf(") ");

            if (s->body->is_braced){
                printf("{\n");
                stmt_print(s->body, indent + 1);
                for (int i = 0; i < indent; i++){
                    printf("\t");
                }
                printf("} ");
            }
            else{
                printf("\n");
                stmt_print(s->body, indent + 1);
            }

            if (s->else_body){
                if (!s->body->is_braced){
                    for (int i = 0; i < indent; i++){
                        printf("\t");
                    }
                }
                printf("else ");
                if (s->else_body->kind != STMT_IF_ELSE && s->else_body->is_braced) {
                    printf("{\n");
                    stmt_print( s->else_body, indent + 1 );
                    for (int i = 0; i < indent; i++){
                        printf("\t");
                    }
                    printf("}\n");
                }
                else if(s->else_body->kind != STMT_IF_ELSE && !s->else_body->is_braced){
                    printf("\n");
                    stmt_print(s->else_body, indent + 1);

                }
                else{
                    stmt_print( s->else_body, indent );

                }
            }
            break;
        case STMT_DECL:
            decl_print(s->decl, indent);
            break;
        default:
            printf("stmt error\n");
            return;

    }
    printf("\n");
    stmt_print(s->next, indent);

}
