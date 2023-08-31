#include "../include/encoder.h"

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

        else if (*curr == '\''){
            printf("Error: char, not string\n");
            return 1;

        }

        else if( *curr != '\"' ){
            printf("Bad string, must start with a double-quote\n");
            return 1;
        }


        else{
            curr++;
            while ( *curr ){

                if ( *curr == '\"' ){

                    if ( decoded_i < size ){
                        if (*(curr+1) != '\0' && isspace( *(curr+1) ) ){
                            decoded_str[ decoded_i++ ] = '\0';
                        }
                        curr++;
                        while( decoded_i++ < size && isspace( *(curr) ) ){
                            curr++;
                        }

                        if (decoded_i >= MAX_STRING_LEN){
                            printf("Error: String is too long!\n");
                            return 1;
                        }
                        else if ( *curr != '\0'){
                            printf("Bad String: Text after end quote!\n");
                            return 1;
                        }

                        int len = sizeof(decoded_str);
                        strncpy(s, decoded_str, len);
                        return 0;


                    }
                    else{
                        printf("Bad String: Text after end quote!\n");
                        return 1;
                    }

                }

                else if ( *curr == '\\' ){
                    curr++;

                    if (*curr == 'a'){
                        decoded_str[ decoded_i++ ] = '\a';
                    }
                    else if (*curr == 'b'){
                        decoded_str[ decoded_i++ ] = '\b';
                    }
                    else if (*curr == 'e'){
                        decoded_str[ decoded_i++ ] = '\e';
                    }
                    else if (*curr == 'f'){
                        decoded_str[ decoded_i++ ] = '\f';
                    }
                    else if ( *curr == 'n' ){
                        decoded_str[ decoded_i++ ] = '\n';
                    }
                    else if (*curr == 'r'){
                        decoded_str[ decoded_i++ ] = '\r';
                    }
                    else if (*curr == 't'){
                        decoded_str[ decoded_i++ ] = '\t';
                    }
                    else if (*curr == 'v'){
                        decoded_str[ decoded_i++ ] = '\v';
                    }

                    else if ( *curr == '\"' || *curr == '\\' || *curr == '\''){
                        decoded_str[ decoded_i++ ] = *curr;
                    }

                    else if (*curr == '0' && *(curr+1) == 'x'){

                        curr += 2;

                        if
                            ( !isspace( *(curr+2) ) &&
                            !( *(curr+2) == '\\' &&
                            ( *(curr+3) == 'n'  ||
                            *(curr+3) == 't' ||
                            *(curr+3) == 'v' ))
                        ){
                            printf("Error: Hex value too large!\n");
                            return 1;
                        }
                        char hexVal[3] = {'\0', '\0', '\0'};
                        hexVal[0] = *(curr++);
                        hexVal[1] = *(curr);
                        int hexInt = (int)strtol(hexVal, NULL, 16);
                        if (hexInt < 33 || hexInt > 127){
                            decoded_str[ decoded_i++ ] = '0';
                            decoded_str[ decoded_i++ ] = 'x';
                            decoded_str[ decoded_i++ ] = *(curr-1);
                            decoded_str[ decoded_i++ ] = *(curr);
                        }
                        else{
                            decoded_str[ decoded_i++ ] = (char)hexInt;
                        }

                    }

                    else{
                        printf("Bad String: Special charater does not exist!\n");
                        return 1;
                    }

                }
                else if ((int)(*curr) > 31 && (int)(*curr) < 127){
                    decoded_str[ decoded_i++ ] = *curr;
                }
                else{
                    printf("Error: Ascii value out of valid range\n");
                    return 1;
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

    char encoded_str[MAX_STRING_LEN];
    int encoded_i = 0;
    memset(encoded_str, '\0', MAX_STRING_LEN);
    char* curr = (char*)s;
    if ( strlen(s) == 0){

        encoded_str[ encoded_i++ ] = '\"';
        encoded_str[ encoded_i++ ] = '\"';
        encoded_str[ encoded_i ] = '\0';
        strncpy(es, encoded_str, 3);

        return 0;
    }
    else if ( strlen(s) == 1 ){
        encoded_str[ encoded_i++ ] = '\'';
        if ( !( (int)(*curr) > 31 && (int)(*curr) < 127 ) ){
            printf("Error: Bad character input\n");
            return 1;
        }
        encoded_str[ encoded_i++ ] = *curr;

        encoded_str[ encoded_i++ ] = '\'';

        int len = sizeof(encoded_str);
        strncpy(es, encoded_str, len);
        return 0;

    }


    encoded_str[ encoded_i++ ] = '\"';

    while ( *(curr) ){
        char c = *curr;

        if (c == '\n'){
            encoded_str[ encoded_i++ ] = '\\';
            encoded_str[ encoded_i++ ] = 'n';
        }

        else if ( c == '\t' ){
            encoded_str[ encoded_i++ ] = '\\';
            encoded_str[ encoded_i++ ] = 't';
        }
        else if ( c == '\v' ){
            encoded_str[ encoded_i++ ] = '\\';
            encoded_str[ encoded_i++ ] = 'v';
        }
        else if ( c == '\r' ){
            encoded_str[ encoded_i++ ] = '\\';
            encoded_str[ encoded_i++ ] = 'r';
        }

        else if ( c == '\\' ){
            encoded_str[ encoded_i++ ] = '\\';
            encoded_str[ encoded_i++ ] = '\\';
        }

        else if ( c == '\"' ){
            encoded_str[ encoded_i++ ] = '\\';
            encoded_str[ encoded_i++ ] = '\"';
        }

        else if ( c == '\'' ){
            encoded_str[ encoded_i++ ] = '\\';
            encoded_str[ encoded_i++ ] = '\'';
        }

        else if ( (int)(*curr) > 31 && (int)(*curr) < 127 ){
            encoded_str[ encoded_i++ ] = *curr;
        }
        else{
            printf("Error: Invalid characted added to string\n");
            return 1;
        }
        curr++;

    }
    encoded_str[ encoded_i++ ] = '\"';
    encoded_str[ encoded_i ] = '\0';
    int len = sizeof(encoded_str);
    strncpy(es, encoded_str, len);
    return 0;

}
