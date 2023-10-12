/*
Declare %token TOKEN types at the Top of the bison file,
causing them to be automatically generated in parser.tab.h
for use by scanner.c.
*/

%token TOKEN_EOF
%token TOKEN_IDENT
%token TOKEN_INTEGER
%token TOKEN_FLOAT
%token TOKEN_CHAR_LITERAL
%token TOKEN_STRING
%token TOKEN_BOOLEAN
%token TOKEN_EE
%token TOKEN_GE
%token TOKEN_LE
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_STRING_LITERAL
%token TOKEN_INTEGER_LITERAL
%token TOKEN_BOOLEAN_LITERAL
%token TOKEN_ARR
%token TOKEN_SEMICOLON
%token TOKEN_FLOAT_LITERAL
%token TOKEN_INCREMENT
%token TOKEN_DECREMENT
%token TOKEN_NOT
%token TOKEN_MULT
%token TOKEN_ADD
%token TOKEN_SUB
%token TOKEN_DIV
%token TOKEN_ASSIGN
%token TOKEN_MOD
%token TOKEN_EXP
%token TOKEN_GROUPING
%token TOKEN_ARR_SUBSCRIPT
%token TOKEN_FUNC_CALL
%token TOKEN_LT
%token TOKEN_GT

%token TOKEN_AUTO
%token TOKEN_VOID
%token TOKEN_FUNCTION
%token TOKEN_RETURN
%token TOKEN_COLON
%token TOKEN_COMMA
%token TOKEN_PRINT
%token TOKEN_NEGATE
%token TOKEN_IF
%token TOKEN_WHILE
%token TOKEN_FOR
%token TOKEN_ELIF
%token TOKEN_FSLASH
%token TOKEN_OPEN_BRACK
%token TOKEN_CLOSED_BRACK
%token TOKEN_OPEN_BRACE
%token TOKEN_CLOSED_BRACE
%token TOKEN_OPEN_PAR
%token TOKEN_CLOSED_PAR
%token TOKEN_CHAR
%token TOKEN_ELSE
%token TOKEN_NE
%token TOKEN_LPAREN
%token TOKEN_RPAREN


%token TOKEN_ERROR



%{

#include <math.h>
#include <stdio.h>
#include <stdlib.h>






/*
Clunky: Manually declare the interface to the scanner generated by flex.
*/

extern char *yytext;
extern int yylex();
extern int yyerror( char *str );

int parser_result = 0;

%}

%%

/* Here is the grammar: program is the start symbol. */

program             : f_decl_list TOKEN_EOF { parser_result = TOKEN_EOF; return 0; }
                    ;



f_decl_list         : f_declaration f_decl_list {printf("got f_decl_list");}
                    |
                    ;

decl_list_init      : list_decl decl_list
                    |
                    ;

list_decl           : TOKEN_IDENT TOKEN_COLON types
                    ;

decl_list           : TOKEN_COMMA list_decl decl_list
                    |
                    ;

f_declaration       : f_decl {printf("got f decl\n");}
                    | declarations
                    ;

f_decl              : TOKEN_IDENT TOKEN_COLON TOKEN_FUNCTION types TOKEN_OPEN_PAR decl_list_init TOKEN_CLOSED_PAR end_func_decl
                    ;

end_func_decl       : TOKEN_ASSIGN TOKEN_OPEN_BRACK statement_list TOKEN_CLOSED_BRACK { printf("got list\n");}
                    | TOKEN_SEMICOLON {printf("got random func\n");}
                    ;

declarations        : TOKEN_IDENT type end_declaration
                    ;

end_declaration     : TOKEN_ASSIGN expression TOKEN_SEMICOLON
                    | TOKEN_SEMICOLON
                    ;

type                : TOKEN_COLON types
                    ;

types               : arr_type
                    | real_types { printf("gotrealtype\n");}
                    ;

arr_type            : TOKEN_ARR TOKEN_OPEN_BRACE expr TOKEN_CLOSED_BRACE types
                    ;

real_types          : TOKEN_INTEGER
                    | TOKEN_FLOAT
                    | TOKEN_BOOLEAN
                    | TOKEN_AUTO
                    | TOKEN_STRING
                    | TOKEN_CHAR
                    | TOKEN_VOID
                    ;

o_statement_list    : statement_list
                    |
                    ;

statement_list      : statement o_statement_list
                    ;

statement           : open_stmt { printf("stmt -> open\n");}
                    | closed_stmt { printf("stmt -> closed\n");}
                    ;



non_if              : TOKEN_OPEN_BRACK statement_list TOKEN_CLOSED_BRACK
                    | non_stmt_list
                    ;

non_stmt_list       : return_expr
                    | non_return
                    ;

non_return          : expression TOKEN_SEMICOLON { printf(" expression parsed\n");}
                    | print_expr TOKEN_SEMICOLON
                    | declarations
                    ;

/*
block               : TOKEN_OPEN_BRACK  TOKEN_CLOSED_BRACK
                    ;
*/

print_expr          : TOKEN_PRINT expr_init
                    ;

return_expr         : TOKEN_RETURN non_return { printf("return\n"); }
                    ;






open_stmt           : TOKEN_IF TOKEN_OPEN_PAR expression TOKEN_CLOSED_PAR closed_stmt { printf(" open( if closed )");}
                    | TOKEN_IF TOKEN_OPEN_PAR expression TOKEN_CLOSED_PAR open_stmt { printf(" open( if open )");}
                    | TOKEN_IF TOKEN_OPEN_PAR expression TOKEN_CLOSED_PAR closed_stmt TOKEN_ELSE open_stmt { printf(" open( if closed else open )");}
                    | TOKEN_FOR TOKEN_OPEN_PAR expr TOKEN_SEMICOLON expr TOKEN_SEMICOLON expr TOKEN_CLOSED_PAR open_stmt { printf("open(for( open))");}
                    ;

closed_stmt         : non_if { printf(" closed(expr)");}
                    | TOKEN_IF TOKEN_OPEN_PAR expression TOKEN_CLOSED_PAR closed_stmt TOKEN_ELSE closed_stmt { printf("closed(if closed else closed)");}
                    | TOKEN_FOR TOKEN_OPEN_PAR expr TOKEN_SEMICOLON expr TOKEN_SEMICOLON expr TOKEN_CLOSED_PAR closed_stmt { printf("closed(for( closed ))");}
                    ;

expr_init           : expression expr_list
                    |
                    ;

expr_list           : TOKEN_COMMA expression expr_list
                    |
                    ;


expr                : expression {printf(" expression -> expr");}
                    |
                    ;


expression          : expression TOKEN_ASSIGN e1
                    | e1
                    ;


e1                  : e1 TOKEN_ADD n1 { printf("add"); }
                    | e1 TOKEN_SUB n1 {printf("sub\n"); }
                    | e1 logic n1 { printf("logic\n"); }
                    | n1                { }
                    ;

n1                  : n1 TOKEN_MULT unary {  printf("mult\n");}
                    | n1 TOKEN_DIV  unary {  printf("div\n");}
                    | n1 TOKEN_AND  unary { printf("and\n"); }
                    | n1 TOKEN_EXP unary { printf("got exponent\n");}
                    | unary { }
                    ;

unary               : TOKEN_ADD unary { printf("got pos\n");  }
                    | TOKEN_SUB unary { printf("got neg\n");  }
                    | TOKEN_NOT unary { printf("got not\n"); }
                    | incr {}
                    ;

incr                : value TOKEN_INCREMENT { printf("incr\n"); }
                    | value TOKEN_DECREMENT {  printf("decr \n");}
                    | value { }
                    ;

value               : TOKEN_INTEGER_LITERAL { }
                    | TOKEN_FLOAT_LITERAL {   }
                    | func_expr
                    | TOKEN_CHAR_LITERAL { printf("gotchar\n"); }
                    | TOKEN_STRING_LITERAL { printf("gotstr\n"); }
                    | TOKEN_BOOLEAN_LITERAL { printf("got boolean\n");}
                    | TOKEN_OPEN_BRACK expr_init TOKEN_CLOSED_BRACK
                    ;

func_expr           : TOKEN_IDENT param_init
                    | param_list
                    ;

index_list          : TOKEN_OPEN_BRACE expression TOKEN_CLOSED_BRACE param_init

param_init          : param_list
                    | index_list
                    |
                    ;

param_list          : TOKEN_OPEN_PAR expr_init TOKEN_CLOSED_PAR
                    ;

logic               : TOKEN_EE{}
                    | TOKEN_NE{}
                    | TOKEN_GE{}
                    | TOKEN_LE{}
                    | TOKEN_GT{}
                    | TOKEN_LT{}
                    | TOKEN_OR{}
                    ;


%%

/*
This function will be called by bison if the parse should
encounter an error.  In principle, "str" will contain something
useful.  In practice, it often does not.
*/

int yyerror( char *str )
{
	printf("parse error: %s\n",str);
	return 0;
}