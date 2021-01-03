#include "util.h"
#include "generation/symtable.h"
#include <stdlib.h>
#include <string.h>

extern void yylex_destroy();

static struct node_t *garbage[GB_EXPRLIST + 1];

void gb_set(int id, struct node_t *list) {
	garbage[id] = list;
}

void clean_exit(int exitcode) {
	for (int id = 0; id < GB_EXPRLIST + 1; id++) {
		node_destroy(garbage[id], 0);
	}

	st_destroy();
	yylex_destroy();
	destroy_scanner();

	exit(exitcode);
}
