#include "encoder.h"

int string_decode( const char *es, char *s ){

    char* curr = es;
    
    char decoded_str[ strlen(es) - 2 ];
    int decoded_i = 0;

    while( *curr ){
        if ( isspace(*curr) ){
            curr++;
            continue;
        }

        else if( *curr != '\"' ){
            printf("Bad string, must start with a double-quote\n");
            return 1;

        else{
            curr++;
            while ( *curr ){

                if ( *curr == '\"' ){

                    if ( !(*(curr+1)) ){
                        decoded_str[ decoded_i ] = '\0';
                        s = &decoded_str;
                        return 0;

                    }
                    else{
                        printf("Bad String: Text after end quote\n");
                        return 1;
                    }

                }
                else{
                    decoded_str[ decoded_i ] = *curr;
                    decoded_i++;
                }
                curr++;
                


            }


        }





    }

}




int string_encode( const char *s, char *es ){
    
}
