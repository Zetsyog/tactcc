%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "generation/defs.h"
    #include "grammar.h"
    #include "util.h"
%}

integer [0-9]+
ident [A-Za-z][A-Za-z0-9]*
string \"(\\.|[^"\\])*\"

%x COMMENT
%option yylineno

%option nounput
%option noinput

%%

"(*" { BEGIN(COMMENT); }
<COMMENT>"*)" { BEGIN(INITIAL); }
<COMMENT>\n   {}
<COMMENT>.    {}

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
"end"      return(END_TOK);
"read"     return(READ);
"write"    return(WRITE);
"of"       return(OF);
"do"       return(DO);
"and"      return(AND);
"or"       return(OR);
"xor"      return(XOR);
"not"      return(NOT);
"true"     return(TRUE);
"false"    return(FALSE);
{string}   {
    memset(yylval.strVal, 0, SYM_NAME_MAX_LEN);
    unsigned int len = strlen(yytext);
    if(len > SYM_NAME_MAX_LEN) {
        len = SYM_NAME_MAX_LEN;
    } else {
        len -= 2;
    }
    strncpy(yylval.strVal, yytext + 1, len);
    return(STR_CST);
}

"+"|"-"|"*"|"/"|"<"|"<="|">"|">="|"="|"<>"  {return yytext[0];}
{ident} {
    memset(yylval.strVal, 0, SYM_NAME_MAX_LEN);
    strncpy(yylval.strVal, yytext, SYM_NAME_MAX_LEN);
    return(IDENT);
}
{integer} {
    yylval.intVal=atoi(yytext);
    return(INT);
}
[[:space:]]+
%%
