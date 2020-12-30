#include "util.h"
#include <string.h>
#include <stdlib.h>

struct expr_val_t *expr_clone(struct expr_val_t expr) {
	struct expr_val_t *ret;
	MCHECK(ret = malloc(sizeof(struct expr_val_t)));
	ret->false = expr.false;
	ret->true  = expr.true;
	ret->ptr   = expr.ptr;
	return ret;
}
