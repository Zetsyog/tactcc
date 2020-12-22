%{
    #include <stdio.h>
    #include "./../include/grammar.h"
    #include "./../include/Intermediate_code.h"
    #include "./../include/generation_code.h"
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


program :  PROG IDENT
        ;

vardeclist : /*epsilon*/ {$$ = malloc(sizeof(int));}
            | varsdecl {$$ = $1;}
            | varsdecl ';' vardeclist {$$ = concat($1,$3);}
            ;

varsdecl : VAR identlist ':' typename {$$=$2;}
        ;

identlist : IDENT
          | IDENT ',' identlist { $$ = $1;}
          ;

typename : atomictype { $$.type = $1.type ;}
          | arraytype { $$.type = TYPE_ARRAY;}
          ;

atomictype : UNIT  { $$.type = TYPE_UINT;}
            | BOOL { $$.type = TYPE_BOOL;}
            | INT  { $$.type = TYPE_INT;}
            ;

arraytype : ARRAY '[' rangelist ']' OF atomictype
          ;

rangelist : INTEGER '.' '.' INTEGER
          | INTEGER '.''.' INTEGER ',' rangelist
          ;

fundecllist :/*epsilon*/              { $$ = malloc(sizeof(int));}
            | fundecl ';' fundecllist { $$ = crelist($1);
                                        $$ = $3;
                                      }
            ;

fundecl : FUNC IDENT '(' parlist ')' ':' atomictype vardeclist instr
        ;

parlist : /*epsilon*/     { $$ = malloc(sizeof(int));}
        | par             { $$ = crelist($1);}
        | par ',' parlist { $$ = crelist($1);
                            $$ = concat($$, $3);
                          }
        ;

par : IDENT ':' typename
    | REF IDENT ':' typename
    ;

instr : IF expr THEN M instr { complete($2.true,$4);
                               $$.next = concat($2.false, $5.next);
                               $$.next = concat($$.next, crelist(nextquad));
                               gencode(GOTO, NULL);
                             }

    | IF expr THEN M instr ELSE N instr { complete($2.True, $4);
                                      complete($2.False, $7.quad);
                                      $$.next = concat($5.next, $8.next);
                                      $$.next = concat($$.next, $7.next);
                                      $$.next = concat($$.next, crelist(nextquad));
                                      gencode(GOTO, NULL);
                                    }

    | WHILE expr DO M instr { complete($3.True, $5);
                              complete($5.next, $2);
                              $$.next = $3.False;
                              gencode(GOTO, $2);
                            }

    | lvalue ':''=' expr { gencode(ASSIGNMENT, $4->name, $1->name);
                           $$.next = NULL;
                         }
    | RETURN expr
    | RETURN
    | IDENT '(' exprlist ')'
    | IDENT '(' ')'
    | BEGIN_TOK sequence END
    | BEGIN_TOK END
    | READ lvalue { gencode(READ,$2->name);
                    $$.next = NULL;
                  }
    | WRITE expr { gencode(WRITE,$2->name);
                   $$.next = NULL;
                 }
    ;

sequence : instr ';' M sequence { complete($1.next,$3);
                                  $$ = $4;
                                }
        | instr ';'             { $$ = $1; }
        | instr                 { $$ = $1; }
        ;

M       : /* emplty */  { $$ = nextquad; }
        ;
N       :  { $$.next = crelist(gencode(GOTO, NULL));
             $$.quad = nextquad;
           }
        ;


lvalue : IDENT
        | IDENT '[' exprlist ']'
        ;

exprlist : expr
        | expr ',' exprlist
        ;
expr : INT                      { $$.type = TYPE_INT;}
      | '(' expr ')'            { $$ = $2;}
      | expr opb expr
      | opu expr
      | IDENT '('exprlist ')'
      | IDENT '(' ')'
      | IDENT '[' exprlist ']'
      | IDENT
      ;
opb :'+'    { $$ = ADD; }
    |'-'    { $$ = MINUS; }
    |'*'    { $$ = MULTIPLIES; }
    |'/'    { $$ = DIVIDES; }
    |'<'    { $$ = LOWER; }
    |'<''=' { $$ = LOWER_OR_EQUAL;}
    |'>'    { $$ = SUPERIOR; }
    |'>''=' { $$ = SUPERIOR_OR_EQUAL; }
    |'='    { $$ = EQUALS; }
    |'<''>' { $$ = DIFFERENT; }
    | AND   { $$ = AND; }
    | OR    { $$ = OR; }
    | XOR   { $$ = XOR; }
    ;
opu : '-' { $$ = NEGATE;}
  | NOT { $$ = NOT;}
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
