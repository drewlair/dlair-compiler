%{
#include "parser.h"

%}
SPECIAL_CHARS   (\\n|\\a|\\b|\\e|\\f|\\r|\\t|\v|\\|\0)
SPACES          [\t\ \n]
BAD_CHARS       [\|\\\#\&\@]
%%

(\ |\n|\t)             { /* do nothing */ }
array               { return TOKEN_ARR; }
true|false           { return TOKEN_BOOLEAN_LITERAL; }
string               { return TOKEN_STRING; }
boolean            { return TOKEN_BOOLEAN; }
float                { return TOKEN_FLOAT; }
void                 { return TOKEN_VOID; }
integer              { return TOKEN_INTEGER; }
while                { return TOKEN_WHILE; }
for                  { return TOKEN_FOR; }
return               { return TOKEN_RETURN; }
print                { return TOKEN_PRINT; }
function             { return TOKEN_FUNCTION; }
char                 { return TOKEN_CHAR; }
auto                 { return TOKEN_AUTO; }
if                   { return TOKEN_IF; }
else                 { return TOKEN_ELSE; }
&&                   { return TOKEN_AND; }
\|\|                   { return TOKEN_OR; }
\!                    { return TOKEN_NOT; }
==                   { return TOKEN_EE; }
\!=                   { return TOKEN_NE; }
>=                   { return TOKEN_GE; }
\<=                   { return TOKEN_LE; }
>                    { return TOKEN_GT; }
\<                    { return TOKEN_LT; }
;                   { return TOKEN_SEMICOLON; }
:                    { return TOKEN_COLON; }
\^                    { return TOKEN_EXP; }
,                    { return TOKEN_COMMA; }
\/\/[^\n]*\n        { /* CPP COMMENT */ }
\/\*([^*]|\*+[^\/])*\*+\/  { /* C Comment */ }
\/                   { return TOKEN_DIV; }
\*                   { return TOKEN_MULT; }
\+\+                 { return TOKEN_INCREMENT; }
\-\-                 { return TOKEN_DECREMENT; }
\+                   { return TOKEN_ADD; }
\-                   { return TOKEN_SUB; }
\[                   { return TOKEN_OPEN_BRACE; }
\]                   { return TOKEN_CLOSED_BRACE; }
\{                   { return TOKEN_OPEN_BRACK; }
\}                   { return TOKEN_CLOSED_BRACK; }
\(                   { return TOKEN_OPEN_PAR; }
\)                   { return TOKEN_CLOSED_PAR; }
%                   { return TOKEN_MOD; }
=                   { return TOKEN_ASSIGN; }
[0-9]{0,19}((\.[0-9]+([eE]-?([0-9]|1[01]))?)|([eE]-?([0-9]|1[01])))  { return TOKEN_FLOAT_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]* { return TOKEN_IDENT; }
[0-9]{1,19}            { return TOKEN_INTEGER_LITERAL; }
\'([a-zA-Z0-9]|\\.)\'    { return TOKEN_CHAR_LITERAL; }
\"([^\"\\\n]+\\.|[^\"\n])*\"      { return TOKEN_STRING_LITERAL; }
<<EOF>>              { return TOKEN_EOF; }
.                  { return TOKEN_ERROR; }


%%

int yywrap() { return 1; }
