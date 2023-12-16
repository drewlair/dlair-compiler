#include "../include/codegen.h"

int codegen_result = 0;
char *currFunctionEpilogue = NULL;

struct num_params{
    int num;
    struct num_params *prev;
    char *func_name;
};

struct num_params *numParams = NULL;




int decl_codegen( struct decl* d ){
    if (!d){
        printf("\n");
        return codegen_result;
    }
    struct type *t;
    if (d->symbol->type->kind == TYPE_FUNCTION){
        
        if (!d->code){
            printf(".text\n");
            printf(".global %s\n", d->name);
        } else{

            printf(".text\n");
            printf(".global %s\n.global %s_epilogue\n", d->symbol->name, d->symbol->name);
            //printf(".global %s_epilogue\n", d->symbol->name);
            printf("%s:\n", d->symbol->name);
            printf("\tpushq %%rbp\n");
            printf("\tmovq %%rsp, %%rbp\n");

            int param_len = param_list_length(d->symbol->type->params);
            for (int i = 0; i < param_len; i++) printf("\tpushq %s\n", param_name(i));
            if (!numParams){
                numParams = malloc(sizeof(struct num_params));
                numParams->num = param_len;
                numParams->prev = NULL;
                numParams->func_name = strdup(d->name);
            } else{
                struct num_params *new = malloc(sizeof(struct num_params));
                new->num = param_len;
                new->prev = numParams;
                new->func_name = strdup(d->name);
                numParams = new;
            }

            printf("\tsubq $%d, %%rsp\n", d->symbol->which*8);

            
            printf("\tpushq %%rbx\n");
            printf("\tpushq %%r12\n");
            printf("\tpushq %%r13\n");
            printf("\tpushq %%r14\n");
            printf("\tpushq %%r15\n");
            if (currFunctionEpilogue) free(currFunctionEpilogue);
            
            currFunctionEpilogue = label_name_create(label_create(), d->name);

            if (d->code) stmt_codegen(d->code);
            else printf("\tcall %s\n", d->name);
            
            
            printf("%s:\n", currFunctionEpilogue);
            if (d->code) printf("\taddq $%d, %%rsp\n", d->symbol->which*8);
            printf("\tpopq %%r15\n");
            printf("\tpopq %%r14\n");
            printf("\tpopq %%r13\n");
            printf("\tpopq %%r12\n");
            printf("\tpopq %%rbx\n");

            for (int i = 0; i < param_len; i++) printf("\tpopq %s\n", param_name(i));

            struct num_params *del = numParams;
            numParams = numParams->prev;
            free(del);

            printf("\tmovq %%rbp, %%rsp\n");
            printf("\tpopq %%rbp\n");
            printf("ret\n");
        }

    } else if (d->symbol->kind == SYMBOL_LOCAL){
            
            if (d->value){
                expr_codegen(d->value);
                t = expr_typecheck(d->value);
                if (t->kind == TYPE_FLOAT){
                    printf("\tmovsd %s, %d(%%rbp)\n", float_name(d->value->reg), (numParams->num + d->symbol->which) * -8);
                    float_free(d->value->reg);
                }else{
                    printf("\tmovq %s, %d(%%rbp)\n", scratch_name(d->value->reg), (numParams->num + d->symbol->which) * -8);
                    scratch_free(d->value->reg);
                }
            }
            
            
    } else{
        printf(".data\n");
        printf(".global %s\n", d->symbol->name);
        printf("%s:\n", d->symbol->name);
        if (d->symbol->type->kind == TYPE_STRING){
            printf("\t.string %s\n", d->value->string_literal);
        } else if (d->symbol->type->kind == TYPE_ARRAY){
            if (!d->value){
                for (int i = 0; i < d->symbol->type->arr_length->literal_value; i++){
                    printf("\t.quad 0\n");
                }
            }else{  
                expr_codegen(d->value);
            }

            printf("\n");
        } else if (d->symbol->type->kind == TYPE_FLOAT){
            if (d->value) printf("\t.double %lf\n", d->value->float_literal);
            else printf("\t.double 0.0\n");
        }
        else{
            if (d->value) printf("\t.quad %d\n", d->value->literal_value);
            else printf("\t.quad 0\n");
        }
        printf(".text\n");

    }
    
    

    /*
    function method:
    .text
    .gloabl f
    push %rbp
    movq %rsp %rbp
    push each arg register
    make room for locals (subq 8*# of locals)
    push callee saved (rbx, r12 - r15)

    other notes:
    when doing a print, need to do: call print_string (from library.c)
    before making this call, need to declare a string in the data section if a constant one is present
    leaq .StrLabel %reg
    pushq r10
    pushq r11
    movq %reg %rdi (%rdi is the data string arg register for the function)
    call print_string
    popq %r11
    popq %r10
    #for return 0
    movq $0 %reg
    movq %reg %rax
    pop1 r15 - r12 && rbx
    movq %rbp %rsp
    popq %rbp




    .f_epilogue jmp to when you return from the function
    movq %rbp %rsp
    pop %rbp
    RET

    */

    return decl_codegen( d->next );

}


void stmt_codegen( struct stmt *s ){
    if (!s) return;
    
    int t;
    int b;

    switch(s->kind){

        case STMT_EXPR:
            expr_codegen(s->init_expr);
            scratch_free(s->init_expr->reg);
            break;

        case STMT_RETURN:
            expr_codegen(s->init_expr);
            printf("\tmovq %s, %%rax\n", scratch_name(s->init_expr->reg));
            scratch_free(s->init_expr->reg);
            //printf("\tmovq %%r12, %%rdi\npushq %%r10\n pushq %%r11\n call print_integer\n popq %%r11\n popq %%r10\n");
            
            printf("\tjmp %s\n", currFunctionEpilogue);
            
            break;

        case STMT_PRINT:
            expr_codegen(s->init_expr);
            break;

        case STMT_DECL:
            decl_codegen(s->decl);
            break;

        case STMT_FOR:
            //printf("\tsubq $%d, %%rsp\n", s->numLocals*8);
            expr_codegen(s->init_expr);
            if (s->init_expr) scratch_free(s->init_expr->reg);
            t = label_create();
            b = label_create();
            printf(".%s:\n", label_name_create(t, "L"));
            expr_codegen(s->expr);
            if (s->expr){ 
                printf("\tcmp $0, %s\n", scratch_name(s->expr->reg));
                scratch_free(s->expr->reg);
            }
            
            printf("\tje .%s\n", label_name_create(b, "L"));
            stmt_codegen(s->body);
            expr_codegen(s->next_expr);

            if (s->next_expr) scratch_free(s->next_expr->reg);

            printf("\tjmp .%s\n", label_name_create(t, "L"));
            printf(".%s:\n", label_name_create(b, "L"));
            printf("\taddq $%d, %%rsp\n", s->numLocals*8);
            break;

        case STMT_IF_ELSE:
            
            expr_codegen(s->init_expr);
            
            t = label_create();
            b = label_create();
            printf("\tcmp $0, %s\n",scratch_name(s->init_expr->reg));
            scratch_free(s->init_expr->reg);
            printf("\tje .%s\n", label_name_create(t, "L"));
            //printf("\tsubq $%d, %%rsp\n", s->body->numLocals*8);
            stmt_codegen(s->body);
            printf("\taddq $%d, %%rsp\n", s->body->numLocals*8);
            printf("\tjmp .%s\n", label_name_create(b, "L"));
            printf(".%s:\n", label_name_create(t, "L"));
            if (s->else_body){
                //printf("\tsubq $%d, %%rsp\n", s->else_body->numLocals*8);
                stmt_codegen(s->else_body);
                //printf("\taddq $%d, %%rsp\n", s->else_body->numLocals*8);
            }
            printf(".%s:\n", label_name_create(b, "L"));
            
            break;
        case STMT_SCOPE:
            stmt_codegen(s->body);
            break;



        default:
            printf("not yet supported stmt");
            break;
        
    }
    stmt_codegen( s->next );
}

const char* symbol_codegen( struct symbol *s ){
    if (!s) return "\0"; //shouldn't happen but beter safe than sorry

    if (s->kind == SYMBOL_GLOBAL){
        //if (s->type->kind == TYPE_ARRAY) printf("here\n");
        return s->name;
    } else if(s->kind == SYMBOL_LOCAL){
        char str[10];
        sprintf(str, "%d(%%rbp)", (s->which + numParams->num)*-8);
        return strdup(str);
    } else{
        char str[10];
        sprintf(str, "%d(%%rbp)", (s->which) * -8);
        return strdup(str);
    }
    return "\0";
}


void expr_codegen( struct expr *e ){

    int label;
    if (!e) return;
    struct type *t;
    switch (e->kind){

        case EXPR_INTEGER_LITERAL:
            e->reg = scratch_alloc();
            if (e->reg == -1){
                codegen_result = 1;
                return;
            }
            printf("\tmovq $%d, %s\n", e->literal_value, scratch_name(e->reg));
            break;

        case EXPR_FLOAT_LITERAL:
            printf(".data\n");
            const char *flabel = float_label();
            printf(".%s:\n", flabel);
            printf("\t.double %lf\n", e->float_literal);
            printf(".text\n");
            e->reg = float_allocate();
            printf("\tmovsd .%s, %s\n", flabel, float_name(e->reg));
            break;



        case EXPR_CHAR_LITERAL:
            e->reg = scratch_alloc();
            if (e->reg == -1){
                codegen_result = 1;
                return;
            }
            
            printf("\tmovq $%d, %s\n", e->literal_value, scratch_name(e->reg));
            break;

        case EXPR_STRING_LITERAL:
            e->reg = scratch_alloc();
            label = label_create();
            printf(".data\n");
            printf(".%s: .string %s\n", label_name_create(label, "S"), e->string_literal);
            printf(".text\n");
            printf("\tleaq .%s, %s\n", label_name_create(label, "S"), scratch_name(e->reg));
            break;

        case EXPR_BOOLEAN_LITERAL:
            e->reg = scratch_alloc();
            if (e->reg == -1){
                codegen_result = 1;
                return;
            }
            
            printf("\tmovq $%d, %s\n", e->literal_value, scratch_name(e->reg));
            
            break;
        
        case EXPR_IDENT_LITERAL:
            
            
            if (e->symbol->type->kind == TYPE_STRING && e->symbol->kind == SYMBOL_GLOBAL){
                e->reg = scratch_alloc();
                printf("\tleaq %s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
            }else if (e->symbol->type->kind == TYPE_FLOAT){
                e->reg = float_allocate();
                printf("\tmovsd %s, %s\n",symbol_codegen(e->symbol), float_name(e->reg) );
            }
            else{
                e->reg = scratch_alloc();
                printf("\tmovq %s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
            }
            break;

        case EXPR_ADD:
            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\taddsd %s, %s\n", float_name(e->right->reg), float_name(e->left->reg));
                float_free(e->right->reg);
            } else{
                printf("\taddq %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
                scratch_free(e->right->reg);
            }
            e->reg = e->left->reg;
            scratch_free(e->right->reg);
            break;

        case EXPR_SUB:
            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\tsubsd %s, %s\n", float_name(e->right->reg), float_name(e->left->reg));
                float_free(e->right->reg);
            }else{
                printf("\tsubq %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
                scratch_free(e->right->reg);
            }
            e->reg = e->left->reg;
            
            break;

        case EXPR_MUL:
            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\tmulsd %s, %s\n", float_name(e->right->reg), float_name(e->left->reg));
                float_free(e->right->reg);
            }else{
                e->reg = scratch_alloc();
                printf("\tmovq %%rax, %s\n", scratch_name( e->reg ));
                printf("\tmovq %s, %%rax\n", scratch_name( e->left->reg ));
                printf("\timulq %s\n", scratch_name( e->right->reg ));
                printf("\tmovq %%rax, %s\n", scratch_name(e->left->reg));
                printf("\tmovq %s, %%rax\n", scratch_name( e->reg ));
                scratch_free(e->right->reg);
                scratch_free(e->reg);
            }
            e->reg = e->left->reg;
            break;

        case EXPR_DIV:
            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("divsd %s, %s\n", float_name(e->right->reg), float_name(e->left->reg));
                float_free(e->right->reg);
                e->reg = e->left->reg;
            }else{
                printf("\tmovq %s, %%rax\n", scratch_name(e->left->reg));
                printf("\tcqo\n");
                printf("\tidivq %s\n", scratch_name(e->right->reg));
                e->reg = e->left->reg;
                printf("\tmovq %%rax, %s\n", scratch_name(e->reg));
                scratch_free(e->right->reg);
            }
            break;

        case EXPR_EXP:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("\tpushq %%r10\n");
            printf("\tpushq %%r11\n");
            printf("\tmovq %s, %%rdi\n", scratch_name(e->left->reg));
            printf("\tmovq %s, %%rsi\n", scratch_name(e->right->reg));
            printf("\tcall integer_power\n");
            printf("\tpopq %%r11\n");
            printf("\tpopq %%r10\n");
            e->reg = e->left->reg;
            printf("\tmovq %%rax, %s\n", scratch_name(e->left->reg));
            scratch_free(e->right->reg);
            break;

        case EXPR_MOD:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("\tmovq %s, %%rax\n",scratch_name(e->left->reg));
            printf("cqo\n");
            printf("\tidivq %s\n", scratch_name(e->right->reg));
            printf("\tmovq %%rdx, %s\n", scratch_name(e->left->reg));
            e->reg = e->left->reg;
            scratch_free(e->right->reg);

            break;
        
        case EXPR_NEGATE:
            expr_codegen(e->left);
            e->reg = e->left->reg;
            printf("\tnegq %s\n", scratch_name(e->reg));
            break;
        
        case EXPR_POS:
            expr_codegen(e->left);
            e->reg = e->left->reg;
            break;

        case EXPR_AND:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("\tand %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
            e->reg = e->left->reg;
            scratch_free(e->right->reg);
            break;
        
        case EXPR_OR:
            expr_codegen(e->left);
            expr_codegen(e->right);
            printf("\tor %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
            e->reg = e->left->reg;
            scratch_free(e->right->reg);
            break;
        
        case EXPR_EE:
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\tCMPEQSD %s, %s\n", float_name(e->left->reg), float_name(e->right->reg));
                e->reg = scratch_alloc();
                printf("\tcvtss2sd %s, %s\n", float_name(e->left->reg), scratch_name(e->reg));
                float_free(e->left->reg);
                float_free(e->right->reg);
            }else{
                //some nice code worth explaining

                //get the code for both expr's
                expr_codegen(e->left);
                expr_codegen(e->right);
                
                //compare both expr's so that the eflags register is set
                printf("\tcmp %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));

                //get rid of the right reg since it's not needed anymore
                scratch_free(e->right->reg);

                //allocate another reg for the eventual result;
                e->reg = scratch_alloc();

                //move the value 0x0040 into the reg, bitmap will keep the 6th bit of any value with an and instr
                printf("\tmovq $64, %s\n", scratch_name(e->reg));

                //grab the eflags reg and put into the left reg by pushing and popping
                printf("\tpushf\n");
                printf("\tpop %s\n",scratch_name(e->left->reg));

                //do the and to get the value of the EE
                printf("\tand %s, %s\n", scratch_name(e->left->reg), scratch_name(e->reg));

                //shift that value to the 0 bit so that the return is a boolean
                printf("\tshrd $6, %s, %s\n", scratch_name(e->reg), scratch_name(e->reg));
            

                //free the left reg so that only e->reg with the bool is left
                scratch_free(e->left->reg);
            }
            break;

        case EXPR_NE:
            //some nice code worth explaining

                //get the code for both expr's
            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\tCMPNEQSD %s, %s\n", float_name(e->left->reg), float_name(e->right->reg));
                e->reg = scratch_alloc();
                printf("\tcvtss2sd %s, %s\n", float_name(e->left->reg), scratch_name(e->reg));
                float_free(e->left->reg);
                float_free(e->right->reg);
            }else{
                
                //compare both expr's so that the eflags register is set
                printf("\tcmp %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));

                //get rid of the right reg since it's not needed anymore
                scratch_free(e->right->reg);

                //allocate another reg for the eventual result;
                e->reg = scratch_alloc();

                //move the value 0x0040 into the reg, bitmap will keep the 6th bit of any value with an and instr
                printf("\tmovq $64, %s\n", scratch_name(e->reg));

                //grab the eflags reg and put into the left reg by pushing and popping
                printf("\tpushf\n");
                printf("\tpop %s\n",scratch_name(e->left->reg));

                //do the and to get the value of the EE
                printf("\tand %s, %s\n", scratch_name(e->left->reg), scratch_name(e->reg));

                //shift that value to the 0 bit so that the return is a boolean
                printf("\tshrd $6, %s, %s\n", scratch_name(e->reg), scratch_name(e->reg));

                //since not equal, need to convert to the opposite of itself
                printf("\txor $1, %s\n", scratch_name(e->reg));

                //free the left reg so that only e->reg with the bool is left
                scratch_free(e->left->reg);
            }
            break;

        case EXPR_LE:

            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\tCMPLESD %s, %s\n", float_name(e->left->reg), float_name(e->right->reg));
                e->reg = scratch_alloc();
                printf("\tcvtss2sd %s, %s\n", float_name(e->left->reg), scratch_name(e->reg));
                float_free(e->left->reg);
                float_free(e->right->reg);
            }else{
                printf("\tsubq %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
                scratch_free(e->right->reg);
                printf("\tsubq $1, %s\n", scratch_name(e->left->reg));
                printf("\tshr $63, %s\n", scratch_name(e->left->reg));
                e->reg = e->left->reg;
            }
            break;
        
        case EXPR_GE:
            //for the greater than or less than cases, will need to check if SF == OF for eflags

            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\tCMPGESD %s, %s\n", float_name(e->left->reg), float_name(e->right->reg));
                e->reg = scratch_alloc();
                printf("\tcvtss2sd %s, %s\n", float_name(e->left->reg), scratch_name(e->reg));
                float_free(e->left->reg);
                float_free(e->right->reg);
            }else{
                printf("\tsubq %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
                scratch_free(e->left->reg);
                printf("\tsubq $1, %s\n", scratch_name(e->right->reg));
                printf("\tshr $63, %s\n", scratch_name(e->right->reg));
                e->reg = e->right->reg;
            }
            break;
        
        case EXPR_LT:
            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\tCMPLTSD %s, %s\n", float_name(e->left->reg), float_name(e->right->reg));
                e->reg = scratch_alloc();
                printf("\tcvtss2sd %s, %s\n", float_name(e->left->reg), scratch_name(e->reg));
                float_free(e->left->reg);
                float_free(e->right->reg);
            }else{
                printf("\tsubq %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
                scratch_free(e->right->reg);
                printf("\tshr $63, %s\n", scratch_name(e->left->reg));
                e->reg = e->left->reg;
            }

            break;
        
        case EXPR_GT:
            expr_codegen(e->left);
            expr_codegen(e->right);
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                printf("\tCMPEQSD %s, %s\n", float_name(e->left->reg), float_name(e->right->reg));
                e->reg = scratch_alloc();
                printf("\tcvtss2sd %s, %s\n", float_name(e->left->reg), scratch_name(e->reg));
                float_free(e->left->reg);
                float_free(e->right->reg);
            }else{
                printf("\tsubq %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
                scratch_free(e->left->reg);
                printf("\tshr $63, %s\n", scratch_name(e->right->reg));
                e->reg = e->right->reg;
            }

            break;

        case EXPR_NOT:
            expr_codegen(e->left);
            t =expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT) printf("\txor $1, %s\n", float_name(e->left->reg));
            else printf("\txor $1, %s\n", scratch_name(e->left->reg));
            e->reg = e->left->reg;

            break;

        case EXPR_ASSIGN:
            t = expr_typecheck(e->left);
            if (t->kind == TYPE_FLOAT){
                expr_codegen(e->left);
                expr_codegen(e->right);
                printf("\tmovsd %s, %s\n", float_name(e->right->reg), symbol_codegen(e->left->symbol));
                float_free(e->right->reg);
            }else if(e->left->kind == EXPR_INDEX){
                //printf("\tmovq %s, (%s)\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
                e->reg = scratch_alloc();
                printf("\tleaq %s, %s\n", e->left->left->symbol->name, scratch_name(e->reg));
                expr_codegen(e->left->right);
                expr_codegen(e->right);
                printf("\tmovq %s, (%s, %s, 8)\n", scratch_name(e->right->reg), scratch_name(e->reg), scratch_name(e->left->right->reg));
                scratch_free(e->left->right->reg);
                scratch_free(e->reg);
                e->reg = e->right->reg;


            }else{
                expr_codegen(e->left);
                expr_codegen(e->right);
                printf("\tmovq %s, %s\n", scratch_name(e->right->reg), symbol_codegen(e->left->symbol));
                scratch_free(e->right->reg);
            }
            e->reg = e->left->reg;
            break;

        case EXPR_INCR:
            expr_codegen(e->left);
            
            printf("\tincq %s\n", scratch_name(e->left->reg));
            if (e->left && e->left->kind == EXPR_IDENT_LITERAL){
                printf("\tmovq %s, %s\n", scratch_name(e->left->reg), symbol_codegen(e->left->symbol));
            }
            e->reg = e->left->reg;
            

            break;
        case EXPR_DECR:
            expr_codegen(e->left);
            printf("\tdecq %s\n", scratch_name(e->left->reg));
            if (e->left && e->left->kind == EXPR_IDENT_LITERAL){
                printf("\tmovq %s, %s\n", scratch_name(e->left->reg), symbol_codegen(e->left->symbol));
            }
            e->reg = e->left->reg;

            break;

        case EXPR_PAREN:
            expr_codegen(e->left);
            e->reg = e->left->reg;

            break;
        

        case EXPR_PRINT_LIST:
            expr_codegen(e->left);
            printf("\tpushq %%r10\n");
            printf("\tpushq %%r11\n");
            t = expr_typecheck(e->left);
            
            if (t->kind == TYPE_INTEGER){
                printf("\tmovq %s, %%rdi\n", scratch_name(e->left->reg));
                printf("\tcall print_integer\n");
                scratch_free(e->left->reg);
            }
            else if (t->kind == TYPE_CHARACTER){
                printf("\tmovq %s, %%rdi\n", scratch_name(e->left->reg));
                printf("\tcall print_character\n");
                scratch_free(e->left->reg);
            }
            else if (t->kind == TYPE_STRING){
                printf("\tmovq %s, %%rdi\n", scratch_name(e->left->reg));
                printf("\tcall print_string\n");
                scratch_free(e->left->reg);
            }
            else if (t->kind == TYPE_BOOLEAN){
                printf("\tmovq %s, %%rdi\n", scratch_name(e->left->reg));
                printf("\tcall print_boolean\n");
                scratch_free(e->left->reg);
            }
            else if (t->kind == TYPE_STRING){
                printf("\tmovq %s, %%rdi\n", scratch_name(e->left->reg));
                printf("\tcall print_string\n");
                scratch_free(e->left->reg);
            } else if(t->kind == TYPE_FLOAT){
                
                printf("\tmovsd %s, %%xmm0\n", symbol_codegen(e->left->symbol));
                printf("\tmovq $1, %%rax\n");
                printf("\tcall print_float\n");
                
                float_free(e->left->reg);
            }
            printf("\tpopq %%r11\n");
            printf("\tpopq %%r10\n");
            
            expr_codegen(e->right);
            break;

        case EXPR_LIST_ARRAY:
            expr_codegen(e->left);

            break;
        
        case EXPR_LIST:
            if (e->left->kind == EXPR_STRING_LITERAL){
                printf("\t.string %s\n", e->left->string_literal);
                expr_codegen(e->right);
            } else{
                printf("\t.quad %d\n", e->left->literal_value);
                expr_codegen(e->right);
            }

            break;
        
        case EXPR_INDEX:
            expr_codegen(e->right);
            e->reg = scratch_alloc();
            printf("\tleaq %s, %s\n", symbol_codegen(e->left->symbol), scratch_name(e->reg));
            printf("\tmovq (%s, %s, 8), %s\n", scratch_name(e->reg), scratch_name(e->right->reg), scratch_name(e->reg));
            scratch_free(e->right->reg);


        /*
            expr_codegen(e->left);
            e->reg = e->left->reg;
            */
            break;

        case EXPR_INDEX_LIST:
            expr_codegen(e->left);
            e->reg = e->left->reg;
        /*
            expr_codegen(e->right);
            e->reg = scratch_alloc();
            printf("\tleaq %s, %s\n", symbol_codegen(e->left->symbol), scratch_name(e->reg));
            printf("\tmovq (%s, %s, 8), %s\n", scratch_name(e->reg), scratch_name(e->right->reg), scratch_name(e->left->reg));
            scratch_free(e->right->reg);
            e->reg = e->left->reg;
        */
            break;

        case EXPR_CALL:
            expr_codegen(e->right);
            printf("\tpushq %%r10\n");
            printf("\tpushq %%r11\n");
            //printf("\tsubq $%d, %%rsp\n", param_list_length(e->left->symbol->type->params));
            printf("\tcall %s\n", e->left->name);
            printf("\tpopq %%r11\n");
            printf("\tpopq %%r10\n");
            param_free();
            e->reg = scratch_alloc();
            printf("\tmovq %%rax, %s\n", scratch_name(e->reg));
            
            
            break;

        case EXPR_PARAM_LIST:
            expr_codegen(e->left);
            e->reg = param_allocate();
            if (e->reg == -1){
                printf("too many params\n");
                codegen_result = 1;
            }
            printf("\tmovq %s, %s\n",scratch_name(e->left->reg), param_name(e->reg) );
            scratch_free(e->left->reg);
            expr_codegen(e->right);

            break;


        default:
            printf("not added yet %d\n", e->kind);
            if (e->kind == EXPR_OR) puts("what?");

    }
}
