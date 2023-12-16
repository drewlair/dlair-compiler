//#include "../include/typecheck.h" // was typecheck.h
#include "../parser.h"
#include "../include/scope.h"
#include "../include/hash_table.h"
#include "../include/resolve.h"
#include "../include/codegen.h"



extern struct expr* expr_create(expr_t kind, struct expr *left, struct expr *right );
extern FILE* yyin;
extern int yyparse();
extern int yylex();
extern char* yytext;
extern struct decl* parser_result;




int main(int argc, char* argv[]){

    //initialize scope stack and return global
    stack = NULL;
    scope_enter();
    localCount = 0;
    returnTypesHead = NULL;
    returnTypesTail = NULL;
    typechecker_result = 1;
    resolver_result = 0;
    labelNum = 0;
    

    //encode test cases
    if ( argc == 3 && ( strcmp(argv[1],"--encode") == 0 ) ){

        //initialize static arrays for file input
        char es[MAX_STRING_LEN+2];
        char check_S[MAX_STRING_LEN];
        char check_ES[MAX_STRING_LEN+2];
        memset(check_S, '\0', MAX_STRING_LEN);
        memset(check_ES, '\0', MAX_STRING_LEN+2);

        //get filename from args
        FILE* fp = fopen( argv[2], "r" );

        //get encoded string
        fgets(es, MAX_STRING_LEN+2, fp);

        //if there is not newline, could be invalid input altogether
        if ( es[ strlen(es) - 1  ] == '\n' ){
            es[ strlen(es) - 1  ] = '\0';
        }
        else{
            printf("Bminor String length exceeded\n");
            return 1;
        }

        printf("Initial Encoded String: %s\n", es);


        if ( string_decode(es, check_S) == 1 ){
            printf("Decoder Test failed\n");
            printf("%s$ %s$\n",es,check_S);
            return 1;
        }

        if ( string_encode(check_S, check_ES) == 1 ){
            printf("Encoder Test failed\n");
            printf("%s$ %s$\n",check_S,check_ES);
            return 1;
        }



        printf("Final Encoded Str: %s\n", check_ES);
        return 0;


    }
    else if ( argc == 3 && ( strcmp(argv[1], "--scan") == 0 ) ){

        yyin = fopen(argv[2], "r");
        printf("eof is %d\n", TOKEN_FLOAT);
        if ( !yyin ){
            printf("Error: Could not Open Test File!\n");
            return 1;
        }

        while ( true ){
            int t = yylex();
            if ( t == TOKEN_EOF ){ break;}

            printf("token: %d  text: <%s>\n", t, yytext);
            if( t == TOKEN_ERROR ){
                printf("Error: Token not valid\n");
                return 1;
            }

        }

        return 0;
    }
    else if ( argc == 3 && ( strcmp(argv[1], "--parse") == 0 ) ){


        yyin = fopen(argv[2], "r");

        if ( !yyin ){
            printf("Error: Could not Open Test File!\n");
            return 1;
        }

        while ( true ){
            int t = yylex();
            if ( t == TOKEN_EOF ) break;

            printf("token: %d  text: <%s>\n", t, yytext);
            if( t == TOKEN_ERROR ){
                printf("Error: Token not valid\n");
                return 1;
            }

        }
        rewind(yyin);

        printf("opened file\n");
        while (!feof(yyin)){

            if (yyparse() == 1){
                printf("Parse Error\n");
                fclose(yyin);
                return 1;
            }
        }
        fclose(yyin);
        printf("Parse Successful\n");


        return 0;
    }
    else if(argc == 3 && ( strcmp( argv[1], "--print") == 0) ){

        yyin = fopen(argv[2], "r");

        if ( !yyin ){
            printf("Error: Could not Open Test File!\n");
            return 1;
        }

        while ( true ){
            int t = yylex();
            if ( t == TOKEN_EOF ) break;

            //printf("token: %d  text: <%s>\n", t, yytext);
            if( t == TOKEN_ERROR ){
                printf("Error: Token not valid\n");
                return 1;
            }

        }
        rewind(yyin);


        if (!yyin){
            printf("file pointer error\n");
        }
        while (!feof(yyin)){

            if (yyparse() == 1){
                printf("Parse Error\n");
                fclose(yyin);
                return 1;
            }
        }
        fclose(yyin);


        decl_print(parser_result, 0);

    }
    else if(argc == 3 && ( strcmp( argv[1], "--resolve") == 0) ){

        yyin = fopen(argv[2], "r");

        if ( !yyin ){
            printf("Error: Could not Open Test File!\n");
            return 1;
        }

        while ( true ){
            int t = yylex();
            if ( t == TOKEN_EOF ) break;

            //printf("token: %d  text: <%s>\n", t, yytext);
            if( t == TOKEN_ERROR ){
                printf("Error: Token not valid\n");
                return 1;
            }

        }
        rewind(yyin);


        if (!yyin){
            printf("file pointer error\n");
        }
        while (!feof(yyin)){

            if (yyparse() == 1){
                printf("Parse Error\n");
                fclose(yyin);
                return 1;
            }
        }
        fclose(yyin);

        decl_resolve( parser_result );

        return resolver_result;

    }
    else if ( argc == 3 && strcmp( argv[1], "--typecheck") == 0){
        yyin = fopen(argv[2], "r");

        if ( !yyin ){
            printf("Error: Could not Open Test File!\n");
            return 1;
        }

        while ( true ){
            int t = yylex();
            if ( t == TOKEN_EOF ) break;

            //printf("token: %d  text: <%s>\n", t, yytext);
            if( t == TOKEN_ERROR ){
                printf("Error: Token not valid\n");
                return 1;
            }

        }
        rewind(yyin);


        if (!yyin){
            printf("file pointer error\n");
        }
        while (!feof(yyin)){

            if (yyparse() == 1){
                printf("Parse Error\n");
                fclose(yyin);
                return 1;
            }
        }
        fclose(yyin);

        decl_resolve( parser_result );
        printf("resolver res: %d\n", resolver_result);
        if (resolver_result == 1){
            return 1;
        }

        decl_typecheck( parser_result );

        if (typechecker_result) return 0;
        else return 1;

    }

    else if (argc == 3 && ( strcmp( argv[1], "--codegen" ) == 0) ){
        yyin = fopen(argv[2], "r");

        if ( !yyin ){
            printf("Error: Could not Open Test File!\n");
            return 1;
        }

        while ( true ){
            int t = yylex();
            if ( t == TOKEN_EOF ) break;

            //printf("token: %d  text: <%s>\n", t, yytext);
            if( t == TOKEN_ERROR ){
                printf("Error: Token not valid\n");
                return 1;
            }

        }
        rewind(yyin);


        if (!yyin){
            printf("file pointer error\n");
        }
        while (!feof(yyin)){

            if (yyparse() == 1){
                printf("Parse Error\n");
                fclose(yyin);
                return 1;
            }
        }
        fclose(yyin);

        decl_resolve( parser_result );
        //printf("resolver res: %d\n", resolver_result);
        if (resolver_result == 1){
            return 1;
        }

        decl_typecheck( parser_result );

        if (typechecker_result == 0) return 1;

        return decl_codegen( parser_result );
        

    }

    else if( argc == 2 && ( strcmp( argv[1], "--test") == 0 ) ){

        yyin = fopen("input.bminor", "r");

        if ( !yyin ){
            printf("Error: Could not Open Test File!\n");
            return 1;
        }

        while ( true ){
            int t = yylex();
            if ( t == TOKEN_EOF ) break;

            //printf("token: %d  text: <%s>\n", t, yytext);
            if( t == TOKEN_ERROR ){
                printf("Error: Token not valid\n");
                return 1;
            }

        }
        rewind(yyin);


        if (!yyin){
            printf("file pointer error\n");
        }
        while (!feof(yyin)){

            if (yyparse() == 1){
                printf("Parse Error\n");
                fclose(yyin);
                return 1;
            }
        }
        scope_enter();
        decl_resolve( parser_result );
        
        decl_typecheck( parser_result );
        if (resolver_result == 0){
            printf("successful typecheck\n");
        }
        
        fclose(yyin);

        


    }

    return 0;
}
