%{
    #include <stdio.h>
    int yylex();
    int yyparse();
    void yyerror(char *s);
%}

%token PROG VAR UNIT BOOL INT ARRAY FUNC REF IF THEN ELSE
%token WHILE RETURN BEGIN_TOK READ WRITE IDENT COM
%token END AND OR XOR NOT DO
%token INTEGER OF

%start program

%%


program : PROG IDENT
        ;

vardecllist : /*epsilon*/
            | varsdecl
            | varsdecl ';' vardecllist
            ;

varsdecl : VAR identlist ':' typename
        ;

identlist : IDENT
          | IDENT ',' identlist
          ;

typename : atomictype
          | arraytype
          ;

atomictype : UNIT
            | BOOL
            | INT
            ;

arraytype : ARRAY '[' rangelist ']' OF atomictype
          ;

rangelist : INTEGER '.' '.' INTEGER
          | INTEGER '.''.' INTEGER ',' rangelist
          ;

fundecllist :/*epsilon*/
            | fundecl ';' fundecllist
            ;

fundecl : FUNC IDENT '(' parlist ')' ':' atomictype vardecllist instr
        ;

parlist : /*epsilon*/
        | par
        | par ',' parlist
        ;

par : IDENT ':' typename
    | REF IDENT ':' typename
    ;

instr : IF expr THEN instr
    | IF expr THEN instr ELSE instr
    | WHILE expr DO instr
    | lvalue ':''=' expr
    | RETURN expr
    | RETURN
    | IDENT '(' exprlist ')'
    | IDENT '(' ')'
    | BEGIN_TOK sequence END
    | BEGIN_TOK END
    | READ lvalue | WRITE expr
    ;

sequence : instr ';' sequence
        | instr ';'
        | instr
        ;

lvalue : IDENT
        | IDENT '[' exprlist ']'
        ;

exprlist : expr
        | expr ',' exprlist
        ;
expr : INT
      | '(' expr ')'
      | expr opb expr
      | opu expr
      | IDENT '('exprlist ')'
      | IDENT '(' ')'
      | IDENT '[' exprlist ']'
      | IDENT
      ;
opb : '+'
    | '-'
    | '*'
    | '/'
    | '<'
    | '<' '='
    | '>'
    | '>' '='
    | '='
    | '<' '>'
    |  AND
    |  OR
    |  XOR
    ;

opu : '-'
  | NOT
  ;

%%

int main(void)
{
    yyparse();
}

void yyerror (char *s)
{
    fprintf(stderr,"%s\n",s);
}
