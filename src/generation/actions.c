#include "generation/defs.h"
#include <stdlib.h>

struct list_t *action_assign(struct symbol_t *sym, struct expr_val_t expr) {
	if (expr.a_type != sym->atomic_type) {
		log_syntax_error("syntax error: incompatible types");
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

	while (arg_it != NULL) {
		desc = desc_it->data;
		arg	 = arg_it->data;

		if (desc->atomic_type != arg->atomic_type) {
			log_syntax_error("syntax error: incompatible types");
		}
		gencode(OP_PUSH_ARG, arg);

		desc_it = desc_it->next;
		arg_it	= arg_it->next;
	}
	gencode(OP_CALL, func->fun_desc->quad);
}

struct expr_val_t action_opb(struct expr_val_t arg1, enum operation_t op,
							 struct expr_val_t arg2, unsigned int marker) {
	struct expr_val_t ret;
	if (arg1.a_type == A_INT && arg2.a_type == A_INT) {
		switch (op) {
		case OP_ADD:
		case OP_MINUS:
		case OP_MULTIPLIES:
		case OP_DIVIDES:
		case OP_POWER:
			ret.ptr	   = newtemp(SYM_VAR, A_INT);
			ret.true   = NULL;
			ret.false  = NULL;
			ret.a_type = A_INT;
			gencode(op, arg1.ptr, arg2.ptr, ret.ptr);
			break;
		case OP_LOWER:
		case OP_LOWER_OR_EQUAL:
		case OP_SUPERIOR:
		case OP_SUPERIOR_OR_EQUAL:
		case OP_EQUALS:
		case OP_DIFFERENT:
			ret.a_type = A_BOOL;
			ret.true   = crelist(nextquad);
			ret.false  = crelist(nextquad + 1);
			gencode(op, arg1.ptr, arg2.ptr, NULL);
			gencode(OP_GOTO, NULL);
			break;
		default:
			break;
		}
	} else if (arg1.a_type == A_BOOL && arg2.a_type == A_BOOL) {
		if (op == OP_AND) {
			complete(arg1.true, marker);
			ret.false  = concat(arg1.false, arg2.false);
			ret.true   = arg2.true;
			ret.a_type = A_BOOL;
		}
		// TODO: or & xor
	}
	return ret;
}
