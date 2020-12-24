#include "generation/defs.h"
#include <stdio.h>

#define TMP_VAR_PREFIX "__tmp__"

struct symbol_t *newtemp(enum sym_type_t sym_type,
						 enum atomic_type_t atomic_type, int data) {
	static unsigned int nextTmp = 0;
	char name[SYM_NAME_MAX_LEN] = {0};
    do {
		snprintf(name, SYM_NAME_MAX_LEN - 1, "%s%u", TMP_VAR_PREFIX, nextTmp);
	} while (st_get(name) != NULL);

	struct symbol_t *sym = sym_create(name, sym_type, atomic_type);
	sym->data			 = data;
	st_put(sym->name, sym);

	return sym;
}