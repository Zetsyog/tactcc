%{
#include <stdio.h>
#include "grammar.h"
#include "generation/defs.h"
int yylex();
int yyparse();
void yyerror(const char *s);
%}

%union {
        struct {
                struct list_t *true;
                struct list_t *false;
        } cond;
        struct list_t *pos;
        int quad;
        int intVal;
}

%token PROG VAR UNIT BOOL INT ARRAY FUNC REF IF THEN ELSE
%token WHILE RETURN BEGIN_TOK READ WRITE IDENT COM
%token END AND OR XOR NOT DO OF

%left XOR OR '+' '-'
%left AND '*' '/'
%left '^'
%left OPU NOT

%nonassoc IFEND
%nonassoc ELSE

%start program

%%
program: PROG IDENT vardeclist fundecllist instr
       ;

vardeclist: /*epsilon*/ {}
          | varsdecl {}
          | varsdecl ';' vardeclist {}
          ;

varsdecl: VAR identlist ':' typename {}
        ;

identlist: IDENT
         | IDENT ',' identlist { }
         ;

typename: atomictype {}
        | arraytype { }
        ;

atomictype: UNIT  { }
          | BOOL { }
          | INT  { }
          ;

arraytype: ARRAY '[' rangelist ']' OF atomictype
         ;

rangelist: INT '.' '.' INT
         | INT '.''.' INT ',' rangelist
         ;

fundecllist: /*epsilon*/              { }
           | fundecl ';' fundecllist { }
           ;

fundecl: FUNC IDENT '(' parlist ')' ':' atomictype vardeclist instr
       ;

parlist: /*epsilon*/     { }
       | par             { }
       | par ',' parlist { }
       ;

par: IDENT ':' typename
   | REF IDENT ':' typename
   ;

instr: IF expr THEN M instr { }
     | IF expr THEN M instr ELSE N instr { }
     | WHILE expr DO M instr {  }
     | lvalue ':' '=' expr { }
     | RETURN expr
     | RETURN
     | IDENT '(' exprlist ')'
     | IDENT '(' ')'
     | BEGIN_TOK sequence END { }
     | BEGIN_TOK END
     | READ lvalue { }
     | WRITE expr { }
     ;

sequence: instr ';' M sequence { }
        | instr ';'            { }
        | instr                { }
        ;

M: /* empty */  { }
 ;

N:  { }
 ;


lvalue: IDENT
      | IDENT '[' exprlist ']'
      ;

exprlist: expr
        | expr ',' exprlist
        ;
expr: INT                     { }
    | '(' expr ')'            { }
    | expr opb expr
    | opu expr
    | IDENT '('exprlist ')'
    | IDENT '(' ')'
    | IDENT '[' exprlist ']'
    | IDENT
    ;
opb:'+'    { }
   |'-'    { }
   |'*'    { }
   |'/'    { }
   |'<'    { }
   |'<''=' { }
   |'>'    { }
   |'>''=' { }
   |'='    { }
   |'<''>' { }
   | AND   { }
   | OR    { }
   | XOR   { }
   ;

opu: '-' %prec OPU{ }
   | NOT { }
   ;

%%

int main(void)
{
    yyparse();
}

void yyerror(const char *s)
{
    fprintf(stderr,"%s\n",s);
}
