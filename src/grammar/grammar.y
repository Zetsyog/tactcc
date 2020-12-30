%{
#include <stdio.h>
#include <stdlib.h>
#include "generation/defs.h"
#include "grammar.h"
#include "util.h"
#include "mips/defs.h"
#include "generation/print_code.h"
#include "options.h"
#include <string.h>

int yylex();
int yyparse();
extern int yylineno;
void yyerror(const char *s);
extern void yylex_destroy();
%}

%define parse.error verbose

%union {
    struct list_t *pos;
    struct expr_val_t expr_val;
    struct {
        struct list_t *next;
    } instr_val;
    struct node_t *list;
    unsigned int quad;
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
%token <strVal> IDENT STR_CST

%left XOR OR '+' '-'
%left AND '*' '/'
%left '^'
%left OPU NOT

%type <expr_val> expr
%type <instr_val> instr sequence loop
%type <sym> lvalue fundecl par
%type <a_type> varsdecl atomictype typename arraytype
%type <list> fundecllist vardeclist identlist parlist exprlist funexprlist
%type <quad> M N
%type <operation> opu opb

%nonassoc ELSE

%start program

%%
program: PROG IDENT P vardeclist fundecllist M instr {
            tabQuad[$6].is_main = 1;
            tabQuad[$6].print_label = 1;
       }
       ;

P: /* epsilon */ { st_unshift(); }

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
                $$ = node_create(sym);
                sym = (struct symbol_t *) $$->data;

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
           | fundecl ';' fundecllist { }
           ;

fundecl: FUNC IDENT <sym>{  
            $$ = sym_create($2, SYM_FUN, 0); st_put($$); st_unshift(); 
       } '(' parlist ')' ':' atomictype vardeclist M {
           gencode(OP_POP_ARG, $3);
       } instr {
            $3->atomic_type = $8;
            $3->fun_desc = fun_desc_create($10, $5);
            struct node_t *it = $5;
            while (it != NULL) {
                ((struct symbol_t *)it->data)->str_val = $3->name;
                it = it->next;
            }
            complete($12.next, nextquad);
            gencode(OP_RETURN, NULL);
            st_shift();
            log_debug("HEY");
       }
       ;

parlist: /*epsilon*/     { $$ = NULL; }
       | par             { $$ = node_create($1); }
       | parlist ',' par { 
           node_append($1, $3); 
           $$ = $1;
       }
       ;

par: IDENT ':' typename { 
        $$ = sym_create($1, SYM_PAR, $3);
        st_put($$);
   }
   | REF IDENT ':' typename {}
   ;

loop: WHILE M expr DO M instr {
        complete($3.true , $5);
        complete($6.next , $2);
        $$.next=$3.false;
        gencode(OP_GOTO,$2);
    }
    | IF expr THEN M instr {
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
            $$.next = action_assign($1, $4);
     }
     | RETURN expr
     | RETURN
     | IDENT '(' funexprlist ')' {
            $$.next = NULL;
            struct symbol_t *sym = st_get($1);
            if(sym == NULL) {
                log_error("syntax error: function %s is not declared", $1);
            }
            if(node_length($3) != sym->fun_desc->par_nb) {
                log_error("syntax error: wrong number of args");
            }
            action_call(sym, $3);
            node_destroy($3, 0);
     }
     | IDENT '(' ')' {
            $$.next = NULL;
            struct symbol_t *sym = st_get($1);
            if(sym == NULL) {
                log_error("syntax error: undefined function %s", $1);
            }
            if(sym->sym_type != SYM_FUN) {
                log_error("syntax error: %s is not a function", sym->name);
            }
            if(sym->fun_desc->par_nb != 0) {
                log_error("syntax error: not_enough arguments for func %s", sym->name);
            }
            gencode(OP_CALL, sym->fun_desc->quad);
     }
     | BEGIN_TOK sequence END_TOK { $$.next = $2.next; }
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

funexprlist: expr { 
            $$ = node_create(action_eval_par($1));
        }
        | funexprlist ',' expr { 
            node_append($1, action_eval_par($3));
            $$ = $1;
            ;
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

exprlist: expr { }
        | exprlist ',' expr { }
        ;

expr: INT {
        $$.false = NULL;
        $$.true = NULL;
        $$.ptr = newtemp(SYM_CST, A_INT, $1);
        $$.a_type = A_INT;
        log_debug("expr: %s", ((struct symbol_t *)$$.ptr)->name);
    }
    | STR_CST {
        $$.false = NULL;
        $$.true = NULL;
        $$.ptr = newtemp(SYM_CST, A_STR, $1);
        $$.a_type = A_STR;
    }
    | TRUE {
        $$.ptr = NULL;
        $$.false = NULL;
        $$.true = crelist(nextquad);
        $$.a_type = A_BOOL;
        gencode(OP_GOTO, NULL);
    }
    | FALSE {
        $$.ptr = NULL;
        $$.true = NULL;
        $$.false = crelist(nextquad);
        $$.a_type = A_BOOL;
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
            $$.ptr = newtemp(SYM_VAR, A_INT);
            $$.a_type = A_INT;
            gencode(OP_NEGATE, $2.ptr, $$.ptr);
        } else if($2.ptr->atomic_type == A_BOOL && $1 == OP_NOT) {
            $$.true = $2.false;
            $$.false = $2.true;
            $$.a_type = A_BOOL;
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
        $$.a_type = sym->atomic_type;
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
    log_error("at line %i:\n\t%s", yylineno, s);
}
