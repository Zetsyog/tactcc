%{

#include "generation/defs.h"
#include "generation/print_code.h"
#include "grammar.h"
#include "options.h"
#include "mips/defs.h"
#include "util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define YYERROR_VERBOSE 1

int yylex();
int yyparse();
extern int yylineno;
void yyerror(const char *s);
%}
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
    struct {
        struct list_t *next;
        unsigned int quad;
    } marker_n;
}

%token PROG VAR REF BEGIN_TOK END_TOK ASSIGN
%token ARRAY OF FUNC RETURN
%token WHILE IF THEN ELSE DO READ WRITE
%token AND OR XOR NOT
%token TRUE FALSE
%token EQUAL NOT_EQUAL LOWER_EQUAL UPPER_EQUAL
%token UNIT BOOL
%token <intVal> INT
%token <strVal> IDENT STR_CST

%type <expr_val> expr op
%type <instr_val> instr sequence loop
%type <sym> lvalue fundecl par K
%type <a_type> varsdecl atomictype typename arraytype
%type <list> fundecllist vardeclist identlist parlist exprlist funexprlist
%type <quad> M
%type <marker_n> N

%left XOR OR '+' '-'
%left AND '*' '/'
%left '^'
%left '<' UPPER_EQUAL '>' LOWER_EQUAL
%left EQUAL NOT_EQUAL
%left OPU NOT

%nonassoc NO_ELSE
%nonassoc ELSE

%start program

%%
program: PROG IDENT { st_unshift(); } vardeclist fundecllist M instr {
            tabQuad[$6].is_main = 1;
            tabQuad[$6].print_label = 1;
            tabQuad[$6].fun_entry = NULL;
            complete($7.next, nextquad);
            gencode(OP_EXIT, NULL);
       }
       ;

vardeclist: /*epsilon*/ { $$ = NULL; }
          | varsdecl {}
          | varsdecl ';' vardeclist {}
          ;

varsdecl: VAR identlist ':' typename {
            if($4 == A_UNIT)
                log_syntax_error("syntax error: var cannot be of type UNIT");

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

fundecllist: /*epsilon*/             { $$ = NULL; }
           | fundecl ';' fundecllist { }
           ;

K: /* epsilon */ {
 }
 ;
 
fundecl: FUNC IDENT K {
            $3 = sym_create($2, SYM_FUN, 0);
            st_put($3);
            st_unshift();
       } '(' parlist ')' ':' atomictype {
            if(node_length($6) > 4) {
                log_syntax_error("syntax error: functions can take at maximum 4 parameters");
            }
            $3->atomic_type = $9;
            $3->fun_desc = fun_desc_create(0, $6);
            struct node_t *it = $6;
            while (it != NULL) {
                ((struct symbol_t *)it->data)->str_val = $3->name;
                it = it->next;
            }
            node_shift(&garbage);
       } vardeclist M {
            struct node_t *it = $6;
            while(it != NULL) {
                gencode(OP_POP_ARG, it->data);
                it = it->next;
            }
       } instr {
            tabQuad[$12].fun_entry = $3;
            $3->fun_desc->quad = &tabQuad[$12];
            complete($14.next, nextquad);
            gencode(OP_RETURN, NULL);
            $3->fun_desc->sym_list = st_shift();
       }
       ;

parlist: /*epsilon*/     { $$ = NULL; }
       | par             { $$ = node_create($1); garbage = node_unshift(garbage, $$); }
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
        gencode(OP_GOTO, &tabQuad[$2]);
    }

    | IF expr THEN M instr %prec NO_ELSE {
        complete($2.true, $4);
        $$.next = concat($2.false, $5.next);
        $$.next = concat($$.next, crelist(nextquad));
        gencode(OP_GOTO, NULL);
    }
    | IF expr THEN M instr ELSE N instr {
            complete($2.true , $4);
            complete($2.false , $7.quad);
            $$.next = concat($5.next , $8.next);
            $$.next = concat($$.next , $7.next);
            $$.next = concat($$.next , crelist(nextquad));
            gencode(OP_GOTO, NULL); 
    }
    ;
instr: loop { $$.next = $1.next; }
     | lvalue ASSIGN expr {
            $$.next = action_assign($1, $3);
     }
     | RETURN expr {
            $$.next = NULL;
            struct symbol_t *sym = action_eval_par($2);
            gencode(OP_RETURN, sym);
     }
     | RETURN {
            $$.next = NULL;
            gencode(OP_RETURN, NULL);
     }
     | IDENT '(' funexprlist ')' {
            $$.next = NULL;
            struct symbol_t *sym = st_get($1);

            action_call(sym, $3);
     }
     | IDENT '(' ')' {
            $$.next = NULL;
            struct symbol_t *sym = st_get($1);
            action_call(sym, NULL);
     }
     | BEGIN_TOK sequence END_TOK { $$.next = $2.next; }
     | BEGIN_TOK END_TOK { $$.next = NULL; }
     | READ lvalue {
            $$.next = NULL;
            if($2->atomic_type != A_INT) {
                log_syntax_error("syntax error: invalid write of type %s",
                        atomic_type_str[$2->atomic_type]);
            }
            gencode(OP_READ, $2);
     }
     | WRITE expr {
        $$.next = NULL;
        if($2.a_type == A_BOOL) {
            complete($2.true, nextquad);
            gencode(OP_WRITE, newtemp(SYM_CST, A_STR, "true"));
            $$.next = crelist(nextquad);
            gencode(OP_GOTO, NULL);
            complete($2.false, nextquad);
            gencode(OP_WRITE, newtemp(SYM_CST, A_STR, "false"));
            $$.next = concat($$.next, crelist(nextquad));
            gencode(OP_GOTO, NULL);
        } else if($2.a_type == A_INT || $2.a_type == A_STR) {
            gencode(OP_WRITE, $2.ptr);
        } else {
            log_syntax_error("syntax error: invalid write of type %s", 
                    atomic_type_str[$2.a_type]);
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
    $$.next = crelist(nextquad);
    gencode(OP_GOTO,NULL);
    $$.quad = nextquad;
 }
 ;


lvalue: IDENT {
            struct symbol_t *sym = st_get($1);
            if(sym == NULL) {
                log_syntax_error("syntax error: ident %s not declared", $1);
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
    | op { $$ = $1; }
    | IDENT '(' funexprlist ')' {
        struct symbol_t *sym = st_get($1);
        action_call(sym, $3);

        struct symbol_t *ret = newtemp(SYM_VAR, sym->atomic_type);
        gencode(OP_POP_RET, ret);
        $$.ptr = ret;
        if(ret->atomic_type == A_BOOL) {
            $$.true = crelist(nextquad);
            $$.false = crelist(nextquad + 1);
            struct symbol_t *tmp = newtemp(SYM_CST, A_BOOL, 1);
            gencode(OP_EQUALS, ret, tmp, NULL);
            gencode(OP_GOTO, NULL);
        }
        $$.a_type = sym->atomic_type;
    }
    | IDENT '(' ')' {
        struct symbol_t *sym = st_get($1);
        action_call(sym, NULL);
        struct symbol_t *ret = newtemp(SYM_VAR, sym->atomic_type);
        gencode(OP_POP_RET, ret);
        if(ret->atomic_type == A_BOOL) {
            $$.true = crelist(nextquad);
            $$.false = crelist(nextquad + 1);
            struct symbol_t *tmp = newtemp(SYM_CST, A_BOOL, 1);
            gencode(OP_EQUALS, ret, tmp, NULL);
            gencode(OP_GOTO, NULL);
        }
        $$.ptr = ret;
        $$.a_type = sym->atomic_type;
    }
    | IDENT '[' exprlist ']' { log_syntax_error("TODO: array"); }
    | IDENT {
        struct symbol_t *sym = st_get($1);
        if(sym == NULL) {
            log_syntax_error("syntax error: ident %s not declared", $1);
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

op: expr '+' M expr { $$ = action_opb($1, OP_ADD, $4, $3); }
   | expr '-' M expr { $$ = action_opb($1, OP_MINUS, $4, $3); }
   | expr '*' M expr { $$ = action_opb($1, OP_MULTIPLIES, $4, $3); }
   | expr '/' M expr { $$ = action_opb($1, OP_DIVIDES, $4, $3); }
   | expr '^' M expr { $$ = action_opb($1, OP_POWER, $4, $3); }
   | expr '<' M expr { $$ = action_opb($1, OP_LOWER, $4, $3); }
   | expr LOWER_EQUAL M expr { $$ = action_opb($1, OP_LOWER_OR_EQUAL, $4, $3); }
   | expr '>' M expr { $$ = action_opb($1, OP_MINUS, $4, $3); }
   | expr UPPER_EQUAL M expr { $$ = action_opb($1, OP_SUPERIOR_OR_EQUAL, $4, $3); }
   | expr EQUAL M expr { $$ = action_opb($1, OP_EQUALS, $4, $3); }
   | expr NOT_EQUAL M expr { $$ = action_opb($1, OP_DIFFERENT, $4, $3); }
   | expr AND M expr { $$ = action_opb($1, OP_AND, $4, $3); }
   | expr OR M expr { $$ = action_opb($1, OP_OR, $4, $3); }
   | expr XOR M expr { $$ = action_opb($1, OP_XOR, $4, $3); }
   | '-' expr %prec OPU {
        if($2.ptr->atomic_type == A_INT) {
            $$.ptr = newtemp(SYM_VAR, A_INT);
            $$.true = NULL;
            $$.false = NULL;
            $$.a_type = A_INT;
            gencode(OP_NEGATE, $2.ptr, $$.ptr);
        }
   }
   | NOT expr {
        if($2.ptr->atomic_type == A_BOOL) {
            $$.ptr = NULL;
            $$.true = $2.false;
            $$.false = $2.true;
            $$.a_type = A_BOOL;
        }
   }
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
    if(syntax_error) {
        clean_exit(EXIT_FAILURE);
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

    clean_exit(0);
    return 0;
}

void yyerror(const char *s)
{
    log_syntax_error("At line %i:\n\t%s", yylineno, s);
}
