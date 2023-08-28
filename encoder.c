#include "encoder.h"

int string_decode( const char *es, char *s ){

    char* curr;
    curr = (char*)es;
    int size = strlen(es) - 1;
    char decoded_str[ size ];
    memset(decoded_str, '\0', size);
    int decoded_i = 0;

    while( *curr ){
        if ( isspace(*curr) ){
            curr++;
            continue;
        }

        else if( *curr != '\"' ){
            printf("Bad string, must start with a double-quote\n");
            return 1;
        }

        else{
            curr++;
            while ( *curr ){

                if ( *curr == '\"' ){

                    if ( decoded_i < size && !(*(curr+1)) ){
                        decoded_str[ decoded_i ] = '\0';
                        s = decoded_str;
                        return 0;

                    }
                    else{
                        printf("Bad String: Text after end quote!\n");
                        printf("%d %d %c $\n",decoded_i,size,*(curr+1));
                        return 1;
                    }

                }
                else if ( *curr == '\\' ){
                    curr++;
                    if ( *curr == 'n' ||
                         *curr == 't' ||
                         *curr == 'v' ||
                         *curr == 'f' ||
                         *curr == 'r' 
                       ){

                        decoded_str[ decoded_i++ ] = '\\';
                        decoded_str[ decoded_i++ ] = *curr;
                    }

                    else if ( *curr == '\"' ){
                        decoded_str[ decoded_i++ ] = *curr;
                    }

                    else{
                        printf("Bad String: Special charater does not exist!\n");
                        return 1;
                    }

                }
                else{
                    decoded_str[ decoded_i++ ] = *curr;
                }
                curr++;


            }

            printf("Bad String: No ending double quote!\n");
            return 1;


        }
        printf("Bad String: Only Leading Spaces");
        return 1;
    }
    printf("Bad String: Completely empty");
    return 1;

}




int string_encode( const char *s, char *es ){
    return 0;
    
}
