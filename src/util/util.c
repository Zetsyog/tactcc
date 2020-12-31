#include "util.h"
#include "generation/symtable.h"
#include <string.h>
#include <stdlib.h>

extern void yylex_destroy();

struct node_t *garbage = NULL;

struct expr_val_t *expr_clone(struct expr_val_t expr) {
	struct expr_val_t *ret;
	MCHECK(ret = malloc(sizeof(struct expr_val_t)));
	ret->false = expr.false;
	ret->true  = expr.true;
	ret->ptr   = expr.ptr;
	return ret;
}

void clean_exit(int exitcode) {
	struct node_t *it = garbage;
	while(it != NULL) {
		node_destroy(it->data, 0);
		it = it->next;
	}
	node_destroy(garbage, 0);

	st_destroy();
	yylex_destroy();
    destroy_scanner();

	exit(exitcode);
}
