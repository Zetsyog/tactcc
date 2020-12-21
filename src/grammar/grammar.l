%{
    #include <stdio.h>
    #include <stdlib.h>

    #include "grammar.h"
    extern int yylval;

%}

integer [0-9]+
ident [A-Za-z][A-Za-z0-9]*
commentaire "(*"(.|\n)*"*)"

%%

[;,:().]  return(yytext[0]);

"program"  return(PROG);
"var"      return(VAR);
"unit"     return(UNIT);
"bool"     return(BOOL);
"int"      return(INT);
"array"    return(ARRAY);
"function" return(FUNC);
"ref"      return(REF);
"if"       return(IF);
"then"     return(THEN);
"else"     return(ELSE);
"while"    return(WHILE);
"return"   return(RETURN);
"begin"    return(BEGIN_TOK);
"end"      return(END);
"read"     return(READ);
"write"    return(WRITE);
"of"       return(OF);
"do"       return(DO);
"and"      return(AND);
"or"       return(OR);
"xor"      return(XOR);
"not"      return(NOT);       
"+"|"-"|"*"|"/"|"<"|"<="|">"|">="|"="|"<>"  {return yytext[0];}
{ident} yylval=(int)yytext[0]; return(IDENT);
{integer} yylval=atoi(yytext); return(INTEGER);
{commentaire} return(COM);

%%
