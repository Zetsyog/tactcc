#include "generation/symbol.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *sym_type_str[] = {[SYM_CST]	= "CONST",
						[SYM_VAR]	= "VAR",
						[SYM_ARRAY] = "ARRAY",
						[SYM_FUN]	= "FUNC",
						[SYM_PAR]	= "PAR"};

char *atomic_type_str[] = {
	[A_UNIT] = "unit", [A_INT] = "int", [A_BOOL] = "bool", [A_STR] = "str"};

struct symbol_t *sym_create(char *name, enum sym_type_t sym_type,
							enum atomic_type_t atomic_type) {
	struct symbol_t *s;
	MCHECK(s = calloc(1, sizeof(struct symbol_t)));
	strncpy(s->name, name, SYM_NAME_MAX_LEN);
	s->sym_type	   = sym_type;
	s->atomic_type = atomic_type;
	s->parent	   = NULL;
	s->depth	   = 0;

	return s;
}

void sym_destroy(struct symbol_t *sym) {
	if (sym == NULL)
		return;

	if (sym->atomic_type == A_STR && sym->sym_type != SYM_PAR) {
		free(sym->str_val);
	}
	if (sym->sym_type == SYM_FUN) {
		fun_desc_destroy(sym->fun_desc);
	}
	free(sym);
}

struct fun_desc_t *fun_desc_create(unsigned int quad,
								   struct node_t *par_sym_list) {
	struct fun_desc_t *desc;
	MCHECK(desc = malloc(sizeof(struct fun_desc_t)));
	desc->quad		   = tabQuad + quad;
	desc->par_nb	   = node_length(par_sym_list);
	desc->par_sym_list = par_sym_list;

	return desc;
}

void fun_desc_destroy(struct fun_desc_t *desc) {
	if(desc == NULL)
		return;
	node_destroy(desc->par_sym_list, 0);
	free(desc);
}