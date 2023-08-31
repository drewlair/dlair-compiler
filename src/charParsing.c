/* Had char stuff working but we can't use it tho
            curr++;
            if (*curr == '\'' && *(curr+1) == '\0'){
                decoded_str[ decoded_i ] = '\0';
                strncpy(s, decoded_str, 1);
                return 0;
            }
            decoded_str[ decoded_i++ ] = '\'';
            if ( *(curr+1) != '\'' || *(curr) == '\\' || *(curr+2) != '\0' ){
                printf("Error: Bad character string\n");
                return 1;
            }

            decoded_str[ decoded_i++ ] = *curr;
            decoded_str[ decoded_i++ ] = '\'';
            decoded_str[ decoded_i ] = '\0';

            int len = strlen(decoded_str)+1;
            strncpy(s, decoded_str, len);
            return 0;
            */
