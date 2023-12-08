#include "../include/label.h"
#include <stdio.h>
#include <stdlib.h>
int label_create(){

    return labelNum++;

}

const char *label_name_create( int label, const char c ){
    char * str = malloc( sizeof(char) * 11 );
    if (c == 's'){
       sprintf(str, "S%d", label); 
    } else{
        sprintf(str, "L%d", label);
    }
    return str;

}



