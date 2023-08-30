#include "encoder.h"



int main(int argc, char* argv[]){

    //encode test cases
    if ( argc == 3 && ( strcmp(argv[1],"--encode") == 0 ) ){

        //initialize static arrays for file input
        char es[MAX_STRING_LEN];
        char check_S[MAX_STRING_LEN];
        char check_ES[MAX_STRING_LEN];
        memset(check_S, '\0', MAX_STRING_LEN);
        memset(check_ES, '\0', MAX_STRING_LEN);

        //get filename from args
        FILE* fp = fopen( argv[2], "r" );

        //get encoded string
        fgets(es, MAX_STRING_LEN, fp);

        //if there is not newline, could be invalid input altogether
        if ( es[ strlen(es) - 1  ] == '\n' ){
            es[ strlen(es) - 1  ] = '\0';
        }
        else{
            printf("Bminor String length exceeded\n");
            printf("%s$\n",es);
            return 1;
        }



        if ( string_decode(es, check_S) == 1 ){
            printf("Decoder Test failed\n");
            printf("%s$ %s$\n",es,check_S);
            return 1;
        }
        else{
            printf("Decoded: %s\n", check_S);
        }

        if ( string_encode(check_S, check_ES) == 1 ){
            printf("Encoder Test failed\n");
            printf("%s$ %s$\n",check_S,check_ES);
            return 1;
        }
        
        else if (strcmp(check_ES, es) != 0){
            printf("Final Encoded String not es\n");
            printf("%s$ %s$\n", es, check_ES);
            return 1;
        }


        printf("success\n");
        return 0;


    }


    return 0;
}
