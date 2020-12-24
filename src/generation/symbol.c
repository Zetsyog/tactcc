#include "generation/symbol.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct symbol_t *sym_create(char *name, enum sym_type_t sym_type,
						  enum atomic_type_t atomic_type) {
	struct symbol_t *s;
	MCHECK(s = calloc(1, sizeof(struct symbol_t)));
	strncpy(s->name, name, SYM_NAME_MAX_LEN);
	s->sym_type = sym_type;
	s->atomic_type = atomic_type;

	return s;
}