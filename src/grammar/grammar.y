%{
#include <stdio.h>
#include <stdlib.h>
#include "generation/defs.h"
#include "grammar.h"
#include "util.h"
#include "mips/defs.h"
#include "generation/print_code.h"
#include "options.h"

int yylex();
int yyparse();
void yyerror(const char *s);
extern void yylex_destroy();
%}

%define parse.error verbose

%union {
    struct list_t *pos;
    struct {
        struct symbol_t *ptr;
        struct list_t *true;
        struct list_t *false;
    } expr_val;
    struct {
        struct list_t *next;
    } instr_val;
    struct node_t *list;
    int quad;
    int intVal;
    char strVal[SYM_NAME_MAX_LEN];
    struct symbol_t *sym;
    unsigned int a_type;
    enum operation_t operation;
}

%token PROG VAR UNIT BOOL ARRAY FUNC REF IF THEN ELSE
%token WHILE RETURN BEGIN_TOK READ WRITE
%token END_TOK AND OR XOR NOT DO OF
%token TRUE FALSE
%token <intVal> INT
%token <strVal> IDENT
%token <strVal> STR_CST

%left XOR OR '+' '-'
%left AND '*' '/'
%left '^'
%left OPU NOT

%type <expr_val> expr
%type <instr_val> instr sequence loop
%type <sym> lvalue
%type <a_type> varsdecl atomictype typename arraytype
%type <list> fundecllist vardeclist identlist parlist
%type <quad> M N
%type <operation> opu opb

%nonassoc IFEND
%nonassoc ELSE

%start program

%%
program: PROG IDENT vardeclist fundecllist instr
       ;

vardeclist: /*epsilon*/ { $$ = NULL; }
          | varsdecl {}
          | varsdecl ';' vardeclist {}
          ;

varsdecl: VAR identlist ':' typename {
            if($4 == A_UNIT)
                log_error("syntax error: var cannot be of type UNIT");

            struct node_t *list = $2;
            struct symbol_t *tmp = (struct symbol_t *)list->data;
            while(tmp != NULL) {
                tmp->atomic_type = $4;
                tmp->sym_type = SYM_VAR;

                st_put(tmp);

                list = list->next;
                if(list == NULL)
                    break;
                tmp = (struct symbol_t *) list->data;
            }
            node_destroy($2, 0);
        }
        ;

identlist: IDENT    {
                log_debug("creating new identlist with %s", $1);
                struct symbol_t *sym = sym_create($1, 0, 0);
                log_debug("sym name: %s", sym->name);
                $$ = node_create(sym);
                sym = (struct symbol_t *) $$->data;
                log_debug("sym name2: %s", sym->name);

            }
         | identlist ',' IDENT  {
                log_debug("append %s to identlist", $3);
                node_append($1, sym_create($3, 0, 0));
                $$ = $1;
            }
         ;

typename: atomictype { $$ = $1; }
        | arraytype {}
        ;

atomictype: UNIT { $$ = A_UNIT; }
          | BOOL { $$ = A_BOOL; }
          | INT  { $$ = A_INT; }
          ;

arraytype: ARRAY '[' rangelist ']' OF atomictype {}
         ;

rangelist: INT '.' '.' INT
         | INT '.''.' INT ',' rangelist
         ;

fundecllist: /*epsilon*/              { $$ = NULL; }
           | fundecl ';' fundecllist { $$ = $3;}
           ;

fundecl: FUNC IDENT '(' parlist ')' ':' atomictype vardeclist M instr
       ;

parlist: /*epsilon*/     {  $$ = NULL; }
       | par             { }
       | par ',' parlist { }
       ;

par: IDENT ':' typename {}
   | REF IDENT ':' typename {}
   ;

loop: WHILE M expr DO M instr {
        complete($3.true , $5);
        complete($6.next , $2);
        $$.next=$3.false;
        gencode(OP_GOTO,$2);
    }
    | IF expr THEN M instr {
        log_debug("Condition quad %u", nextquad);
        complete($2.true, $4);
        $$.next = concat($2.false, $5.next);
        $$.next = concat($$.next, crelist(nextquad));
        gencode(OP_GOTO, NULL);
    }
    | IF expr THEN M instr ELSE N instr {
            /* complete($2.true , $4);
            complete($2.false , $7);
            $$.next = concat($5.next , $8.next);
            $$.next = concat($$.next , $7.next);
            $$.next = concat($$.next , crelist(nextquad));
            gencode(OP_GOTO , NULL); */
    }

instr: loop { $$.next = $1.next; }
     | lvalue ':' '=' expr {
            $$.next = NULL;
            if(($4.ptr != NULL && $1->atomic_type != $4.ptr->atomic_type )
                    || ($1->atomic_type == A_BOOL && $4.ptr != NULL)
                ) {
                log_error("incompatible types");
            }
            if($1->atomic_type == A_BOOL) {
                complete($4.false, nextquad);
                gencode(OP_ASSIGNMENT, newtemp(SYM_CST, A_BOOL, 0), $1);
                $$.next = crelist(nextquad);
                gencode(OP_GOTO, NULL);
                complete($4.true, nextquad);
                gencode(OP_ASSIGNMENT, newtemp(SYM_CST, A_BOOL, 1), $1);
                concat($$.next, crelist(nextquad));
                gencode(OP_GOTO, NULL);
            } else {
                log_debug("calling gencode(:=, %s, %s)",
                    ($4.ptr)->name,
                    $1->name);
                gencode(OP_ASSIGNMENT, $4.ptr, $1);
            }
     }
     | RETURN expr
     | RETURN
     | IDENT '(' exprlist ')'
     | IDENT '(' ')'
     | BEGIN_TOK sequence END_TOK { $$.next = NULL; }
     | BEGIN_TOK END_TOK { $$.next = NULL; }
     | READ lvalue { $$.next = NULL; gencode(OP_READ, $2); }
     | WRITE expr {
        $$.next = NULL;
        if($2.ptr->atomic_type == A_BOOL) {
            complete($2.true, nextquad);
            gencode(OP_WRITE, newtemp(SYM_CST, A_STR, "true"));
            $$.next = crelist(nextquad);
            gencode(OP_GOTO, NULL);
            complete($2.false, nextquad);
            gencode(OP_WRITE, newtemp(SYM_CST, A_STR, "false"));
            $$.next = concat($$.next, crelist(nextquad));
            gencode(OP_GOTO, NULL);
        } else {
            gencode(OP_WRITE, $2.ptr);
        }
     }
     ;

sequence: instr ';' M sequence {
            complete($1.next, $3);
            $$.next = $4.next;
        }
        | instr ';' { $$.next = $1.next; }
        | instr     { $$.next = $1.next; }
        ;

M: /* empty */  { $$ = nextquad; }
 ;

N:  /* empty */  {
/*  $$.next = crelist(nextquad);
    gencode(OP_GOTO,NULL);
    $$ = nextquad; */
 }
 ;


lvalue: IDENT {
            struct symbol_t *sym = st_get($1);
            if(sym == NULL) {
                log_error("syntax error: ident %s not declared", $1);
                exit(1);
            }
            log_debug("lvalue: %s", sym->name);
            $$ = sym;
      }
      | IDENT '[' exprlist ']'  {}
      ;

exprlist: expr
        | expr ',' exprlist
        ;

expr: INT {
        $$.ptr = newtemp(SYM_CST, A_INT, $1);
        log_debug("expr: %s", ((struct symbol_t *)$$.ptr)->name);
    }
    | STR_CST {
        $$.ptr = newtemp(SYM_CST, A_STR, $1);
    }
    | TRUE {
        $$.ptr = NULL;
        $$.true = crelist(nextquad);
        gencode(OP_GOTO, NULL);
    }
    | FALSE {
        $$.ptr = NULL;
        $$.false = crelist(nextquad);
        gencode(OP_GOTO, NULL);
    }
    | '(' expr ')'  {
                        $$ = $2;
                    }
    | expr opb expr {
        $$.ptr = NULL;
    }
    | opu expr {
        if($2.ptr->atomic_type == A_INT && $1 == OP_NEGATE) {
            $$.ptr = $2.ptr;
            $2.ptr->int_val *= -1;
        } else if($2.ptr->atomic_type == A_BOOL && $1 == OP_NOT) {
            $$.true = $2.false;
            $$.false = $2.true;
        }else {
            log_error("syntax: invalid operation");
        }
    }
    | IDENT '('exprlist ')'
    | IDENT '(' ')'
    | IDENT '[' exprlist ']'
    | IDENT {
        struct symbol_t *sym = st_get($1);
        if(sym == NULL) {
            log_error("syntax error: ident %s not declared", $1);
        }
        if(sym->atomic_type == A_BOOL) {
            $$.true = crelist(nextquad);
            $$.false = crelist(nextquad + 1);
            struct symbol_t *tmp = newtemp(SYM_CST, A_BOOL, 1);
            log_debug("Gen IF %s == %u", sym->name, tmp->int_val);
            gencode(OP_EQUALS, sym, tmp, NULL);
            gencode(OP_GOTO, NULL);
        }
        $$.ptr = sym;
    }
    ;

opb:'+'    { $$ = OP_ADD; }
   |'-'    { $$ = OP_MINUS; }
   |'*'    { $$ = OP_MULTIPLIES; }
   |'/'    { $$ = OP_DIVIDES; }
   |'^'    { $$ = OP_POWER; }
   |'<'    { $$ = OP_LOWER; }
   |'<''=' { $$ = OP_LOWER_OR_EQUAL; }
   |'>'    { $$ = OP_SUPERIOR; }
   |'>''=' { $$ = OP_SUPERIOR_OR_EQUAL; }
   |'=''=' { $$ = OP_EQUALS; }
   |'<''>' { $$ = OP_DIFFERENT; }
   | AND   { $$ = OP_AND; }
   | OR    { $$ = OP_OR; }
   | XOR   { $$ = OP_XOR; }
   ;

opu: '-' %prec OPU{ $$ = OP_NEGATE;  }
   | NOT { $$ = NOT; }
   ;

%%

int main(int argc, char **argv)
{
    log_init();
    parse_opt(argc, argv);
    FILE *out = stdout;

    if(options.output_path != NULL) {
        out = fopen(options.output_path, "w");
        if(out == NULL) {
            log_error("Can't open file %s", options.output_path);
            exit(EXIT_FAILURE);
        }
    }

    log_info("Starting compilation");
    st_create(10000);
    nextquad = 0;
    yyparse();
    log_info("done");
    if(options.print_tos) {
        log_debug("Sym table :");
        st_print();
    }
    log_debug("Result :");
    print_intermediate_code();
    if(options.output_path != NULL) {
        log_info("Compiling in file %s", options.output_path);
    } else {
        log_debug("Generated MIPS :");
    }
    gen_mips(out);
    if(options.output_path != NULL) {
        fclose(out);
    }


    st_destroy();
    yylex_destroy();
    return 0;
}

void yyerror(const char *s)
{
    fprintf(stderr,"%s\n",s);
    exit(EXIT_FAILURE);
}
