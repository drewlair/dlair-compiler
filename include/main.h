#include "encoder.h"
#include "symbol.h"
#include <stdbool.h>
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


struct stack_node{
    struct hash_table* ht;
    struct stack_node* prev;
    int size;
    int localCount;
};

struct stack_node* stack;
