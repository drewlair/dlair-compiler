#include "../include/main.h"


int main(int argc, char* argv[]){

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

        return 0;
    }



    return 0;
}
