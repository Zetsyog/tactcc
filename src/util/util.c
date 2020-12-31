#include "util.h"
#include "generation/symtable.h"
#include <string.h>
#include <stdlib.h>

extern void yylex_destroy();

struct node_t *garbage = NULL;

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
