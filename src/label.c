#include "../include/label.h"
#include "../include/scope.h"
#include <stdio.h>
#include <stdlib.h>
int label_create(){

    return labelNum++;

}

char *label_name_create( int label, const char* name ){
    char str[strlen(name) + 11];
    
    sprintf(str, "%s%d", name, label);
    
    while (scope_lookup(str)){
        label = label_create();
        sprintf(str, "%s%d", name, label);
    }

    return strdup(str);

}



