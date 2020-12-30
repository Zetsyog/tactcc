#include "generation/defs.h"
#include <stdlib.h>

struct list_t *action_assign(struct symbol_t *sym, struct expr_val_t expr) {
	if ((expr.ptr != NULL && sym->atomic_type != expr.ptr->atomic_type) ||
		(sym->atomic_type == A_BOOL && expr.ptr != NULL)) {
		log_error("incompatible types");
	}
	if (sym->atomic_type == A_BOOL) {
		struct list_t *ret;
		complete(expr.false, nextquad);
		gencode(OP_ASSIGNMENT, newtemp(SYM_CST, A_BOOL, 0), sym);
		ret = crelist(nextquad);
		gencode(OP_GOTO, NULL);
		complete(expr.true, nextquad);
		gencode(OP_ASSIGNMENT, newtemp(SYM_CST, A_BOOL, 1), sym);
		concat(ret, crelist(nextquad));
		gencode(OP_GOTO, NULL);
		return ret;
	} else {
		log_debug("calling gencode(:=, %s, %s)", (expr.ptr)->name, sym->name);
		gencode(OP_ASSIGNMENT, expr.ptr, sym);
		return NULL;
	}
}

struct symbol_t *action_eval_par(struct expr_val_t expr) {
	struct symbol_t *tmp;
	struct list_t *next;

	tmp	 = newtemp(SYM_VAR, expr.a_type);
	next = action_assign(tmp, expr);
	complete(next, nextquad);

	return tmp;
}

void action_call(struct symbol_t *func, struct node_t *arg_it) {
	struct node_t *desc_it = func->fun_desc->par_sym_list;
	struct symbol_t *desc, *arg;

	while(arg_it != NULL) {
		desc   = desc_it->data;
		arg	   = arg_it->data;

		if(desc->atomic_type != arg->atomic_type) {
			log_error("syntax error: incompatible types");
		}
		gencode(OP_PUSH_ARG, arg);

		desc_it = desc_it->next;
		arg_it	= arg_it->next;
	}
	gencode(OP_CALL, func->fun_desc->quad);
}
