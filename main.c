#include "encoder.h"



int main(int argc, char* argv[]){
    printf("in main\n");
    if ( argc == 3 && ( strcmp(argv[1],"--encode") == 0 ) ){
        char es[MAX_STRING_LEN];
        char s[MAX_STRING_LEN];
        char checkStr[MAX_STRING_LEN];
        //char path2[17] = "test/encode/bad0";
        FILE* fp = fopen( argv[2], "r" );
        fgets(es, MAX_STRING_LEN, fp);
        fgets(s, MAX_STRING_LEN, fp);
        
        if ( es[ strlen(es) - 1  ] == '\n' ){
            es[ strlen(es) - 1  ] = '\0';
        }
        else{
            printf("Bminor String length exceeded\n");
            printf("%s$\n%s$\n",es,s);
            return 1;
        }
        if ( s[ strlen(s) - 1  ] == '\n' ) {
            s[ strlen(s) - 1  ] = '\0';
        }
        else{
            printf("Bminor String length exceeded\n");
            return 1;
        }

        strcpy(checkStr, s);

        if ( string_decode(es, s) == 1){
            printf("Encoder Test failed: String decode\n");
            return 1;
        }

        else if (strcmp(s, checkStr) != 0){
            printf("Encoder Test failed: Original string comparison\n");
            return 1;
        }
        printf("success\n");
        return 0;


        /*
        for (int i = 0; i < 10; i++){

            path1[16] = (char)(48 + i);

            FILE* fp = fopen( path1, "r" );

            fgets(es, MAX_STRING_LEN, fp);
            fgets(s, MAX_STRING_LEN, fp);

            strcpy(checkStr, s);

            if ( string_decode(es, s) == 1){
                printf("Encoder Test %d failed: String decode\n", i);
                fails++;
            }

            else if (strcmp(s, checkStr) != 0){
                printf("Encoder Test %d failed: Original string comparison\n", i);
                fails++;
            }
            memset(es, '\0', strlen(es));
            memset(s, '\0', strlen(s));

        }
        
        for (int i = 0; i < 10; i++){

            path1[16] = (char)(48 + i);

            FILE* fp = fopen( path1, "r" );

            fgets(es, MAX_STRING_LEN, fp);
            fgets(s, MAX_STRING_LEN, fp);

            strcpy(checkStr, s);

            if ( string_decode(es, s) == 1){
                printf("Encoder Test %d failed: String decode\n", i);
                fails++;
            }

            else if (strcmp(s, checkStr) != 0){
                printf("Encoder Test %d failed: Original string comparison\n", i);
                fails++;
            }
            memset(es, '\0', strlen(es));
            memset(s, '\0', strlen(s));

        }
        */


    }


    return 0;
}
