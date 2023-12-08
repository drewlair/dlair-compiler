#include "../include/expr.h"
#include "../include/encoder.h"

extern struct expr* expr_create( expr_t kind, struct expr *left, struct expr *right ){


    struct expr *e = malloc( sizeof(struct expr) );
    e->kind = kind;
    e->left = left;
    e->right = right;





    return e;
}

extern struct expr* expr_create_name( const char* n, expr_t kind ){
    struct expr *e = malloc(sizeof(struct expr));

    e->name = n;
    e->kind = kind;
    e->left = NULL;
    e->right = NULL;

    return e;

}

extern struct expr* expr_create_integer_literal( int c ){
    struct expr* e = malloc( sizeof(struct expr));
    e->literal_value = c;
    e->left = NULL;
    e->right = NULL;
    e->kind = EXPR_INTEGER_LITERAL;

    return e;

}

extern struct expr* expr_create_boolean_literal( const char* str ){
    struct expr *e = malloc(sizeof(struct expr));
    if (strcmp("true",str)==0) e->literal_value = 1;
    else e->literal_value = 0;
    e->left = NULL;
    e->right = NULL;
    e->kind = EXPR_BOOLEAN_LITERAL;

    return e;

}

extern struct expr* expr_create_float_literal( float f ){
    struct expr *e = malloc(sizeof(struct expr));

    e->left = NULL;
    e->right = NULL;
    e->kind = EXPR_FLOAT_LITERAL;
    e->float_literal = f;


    return e;

}

extern struct expr* expr_create_char_literal( const char *c ){
    struct expr *e = malloc(sizeof(struct expr));

    e->left = NULL;
    e->right = NULL;
    e->kind = EXPR_CHAR_LITERAL;
    if (c[1] == '\\'){
        if (c[2] == '\\'){
            e->literal_value = 92; //ascii's for all special chars
        }
        else if (c[2] == 'n'){
            e->literal_value = 10;
        }
        else if (c[2] == ' '){
            e->literal_value = 32;
        }
        else if (c[2] == 't'){
            e->literal_value = 9;
        }
        else if (c[2] == 'a'){
            e->literal_value = 7;
        }
        else if (c[2] == 'b'){
            e->literal_value = 8;
        }
        else if (c[2] == 'e'){
            e->literal_value = 27;
        }
        else if (c[2] == 'f'){
            e->literal_value = 12;
        }
        else if (c[2] == 'r'){
            e->literal_value = 13;
        }
        else if (c[2] == 'v'){
            e->literal_value = 11;
        }
        else if (c[2] == '\''){
            e->literal_value = 39;
        }
        else if (c[2] == '\"'){
            e->literal_value = 34;
        }
        
    }
    else{
        e->literal_value = (int)c[1];
    }



    return e;

}

extern struct expr* expr_create_string_literal( const char *str ){

    struct expr *e = malloc(sizeof(struct expr));

    e->left = NULL;
    e->right = NULL;
    e->kind = EXPR_STRING_LITERAL;
    e->string_literal = str;


    return e;

}


extern void expr_print( struct expr* e ){
    if (!e){
        return;
    }
    switch (e->kind){

        case EXPR_ADD:
            expr_print(e->left);
            printf("+");
            expr_print(e->right);
            break;
        case EXPR_SUB:
            expr_print(e->left);
            printf("-");
            expr_print(e->right);
            break;
        case EXPR_MUL:
            expr_print(e->left);
            printf("*");
            expr_print(e->right);
            break;
        case EXPR_DIV:
            expr_print(e->left);
            printf("/");
            expr_print(e->right);
            break;
        case EXPR_AND:
            expr_print(e->left);
            printf("&&");
            expr_print(e->right);
            break;
        case EXPR_MOD:
            expr_print(e->left);
            printf("%%");
            expr_print(e->right);
            break;
        case EXPR_EE:
            expr_print(e->left);
            printf("==");
            expr_print(e->right);
            break;
        case EXPR_NE:
            expr_print(e->left);
            printf("!=");
            expr_print(e->right);
            break;
        case EXPR_GE:
            expr_print(e->left);
            printf(">=");
            expr_print(e->right);
            break;
        case EXPR_LE:
            expr_print(e->left);
            printf("<=");
            expr_print(e->right);
            break;
        case EXPR_GT:
            expr_print(e->left);
            printf(">");
            expr_print(e->right);
            break;
        case EXPR_LT:
            expr_print(e->left);
            printf("<");
            expr_print(e->right);
            break;
        case EXPR_OR:
            expr_print(e->left);
            printf("||");
            expr_print(e->right);
            break;
        case EXPR_EXP:
            expr_print(e->left);
            printf("^");
            expr_print(e->right);
            break;
        case EXPR_ASSIGN:
            expr_print(e->left);
            printf(" = ");
            expr_print(e->right);
            break;
        case EXPR_NEGATE:
            printf("-");
            expr_print(e->left);
            break;
        case EXPR_POS:
            printf("+");
            expr_print(e->left);
            break;
        case EXPR_NOT:
            printf("!");
            expr_print(e->left);
            break;
        case EXPR_INCR:
            expr_print(e->left);
            printf("++");
            break;
        case EXPR_DECR:
            expr_print(e->left);
            printf("--");
            break;
        case EXPR_LIST_ARRAY:
            printf("{");
            expr_print(e->left);
            printf("}");
            break;
        case EXPR_LIST:
            expr_print(e->left);
            if(e->right){
                printf(", ");
                expr_print(e->right);
            }
            break;
        case EXPR_INDEX_LIST:
            printf("[");
            expr_print(e->left);
            printf("]");
            expr_print(e->right);
            break;
        case EXPR_INDEX:
            expr_print(e->left);
            expr_print(e->right);
            break;
        case EXPR_CALL:
            expr_print(e->left);
            printf("( ");
            expr_print(e->right);
            printf(" )");
            break;
        case EXPR_INTEGER_LITERAL:
            printf("%d", e->literal_value);
            break;
        case EXPR_FLOAT_LITERAL:
            printf("%g",e->float_literal);
            break;
        case EXPR_CHAR_LITERAL:
            printf("\'%c\'",e->literal_value);
            break;
        case EXPR_STRING_LITERAL:
            printf("%s",e->string_literal);

            break;
        case EXPR_BOOLEAN_LITERAL:
            if (e->literal_value == 0){
                printf("false");
            } else {
                printf("true");
            }
            break;
        case EXPR_IDENT_LITERAL:
            printf("%s",e->name);
            break;
        case EXPR_PAREN:
            while (e->left && e->left->kind == EXPR_PAREN){
                e = e->left;
            }
            printf("(");
            expr_print(e->left);
            printf(")");
            break;
        case EXPR_PARAM_LIST:
            expr_print(e->left);
            if (e->right){
                printf(", ");
                expr_print(e->left);
            }

        default:
            printf("bad expr kind %d\n",e->kind);
            return;
    }

}