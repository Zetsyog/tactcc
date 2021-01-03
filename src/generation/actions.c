#include "generation/defs.h"
#include <stdlib.h>

struct list_t *action_assign(struct symbol_t *sym, struct expr_val_t expr) {
	if (expr.a_type != sym->atomic_type) {
		log_syntax_error("syntax error: incompatible types");
	}
	if (sym->atomic_type == A_BOOL) { // If bool assignment
		struct list_t *ret;
		// if expr is false, goto to newly created quad that will assign to sym
		// value 0
		complete(expr.false, nextquad);
		gencode(OP_ASSIGNMENT, newtemp(SYM_CST, A_BOOL, 0), sym);
		ret = crelist(nextquad);
		// Goto next instr
		gencode(OP_GOTO, NULL);
		// if expr is true, goto to newly created quad that will assign to sym
		// value 1
		complete(expr.true, nextquad);
		gencode(OP_ASSIGNMENT, newtemp(SYM_CST, A_BOOL, 1), sym);
		concat(ret, crelist(nextquad));
		// Goto next instr
		gencode(OP_GOTO, NULL);
		return ret;
	} else { // Case of int assignment
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

void action_call(struct symbol_t *sym, struct node_t *arg_list) {
	// List of var passed to function in call
	struct node_t *arg_it = arg_list;
	if (sym == NULL) { // symbol not found in st
		log_syntax_error("syntax error: undefined function %s", sym);
	}
	if (sym->sym_type != SYM_FUN) { // symbol not a function
		log_syntax_error("syntax error: %s is not a function", sym->name);
	}
	if (sym->fun_desc->par_nb !=
		node_length(arg_it)) { // if args number does not match func decl
		log_syntax_error("syntax error: not_enough arguments for func %s",
						 sym->name);
	}

	// List of declared arguments
	struct node_t *desc_it = sym->fun_desc->par_sym_list;
	struct symbol_t *desc, *arg;

	// We know here that arg list and desc list have the same size
	// This first while check that passed args match the function definition
	while (arg_it != NULL) {
		desc = desc_it->data;
		arg	 = arg_it->data;

		// Check if given arg match func prototype
		if (desc->atomic_type != arg->atomic_type) {
			log_syntax_error("syntax error: incompatible types");
		}

		desc_it = desc_it->next;
		arg_it	= arg_it->next;
	}

	while ((arg = node_remove_last(&arg_list)) != NULL) {
		gencode(OP_PUSH_ARG, arg);
	}

	gencode(OP_CALL, sym->fun_desc->quad);
}

struct expr_val_t action_opb(struct expr_val_t arg1, enum operation_t op,
							 struct expr_val_t arg2, unsigned int marker) {
	struct expr_val_t ret;
	if (arg1.a_type == A_INT && arg2.a_type == A_INT) {
		switch (op) {
		// If arithmetical operation
		case OP_ADD:
		case OP_MINUS:
		case OP_MULTIPLIES:
		case OP_DIVIDES:
		case OP_POWER:
			ret.ptr	   = newtemp(SYM_VAR, A_INT);
			ret.true   = NULL;
			ret.false  = NULL;
			ret.a_type = A_INT;
			// Simply store result in tmp var
			gencode(op, arg1.ptr, arg2.ptr, ret.ptr);
			break;
		// If comparison coperation
		case OP_LOWER:
		case OP_LOWER_OR_EQUAL:
		case OP_SUPERIOR:
		case OP_SUPERIOR_OR_EQUAL:
		case OP_EQUALS:
		case OP_DIFFERENT:
			// return type is boolean
			ret.a_type = A_BOOL;
			ret.true   = crelist(nextquad);
			ret.false  = crelist(nextquad + 1);
			// If arg1 op arg2 GOTO ?
			gencode(op, arg1.ptr, arg2.ptr, NULL);
			// GOTO ?
			gencode(OP_GOTO, NULL);
			break;
		default:
			break;
		}
		// If bool operation
	} else if (arg1.a_type == A_BOOL && arg2.a_type == A_BOOL) {
		if (op == OP_AND) {
			// If arg1 is true, then we must evaluate arg2
			complete(arg1.true, marker);
			ret.ptr	   = NULL;
			ret.false  = concat(arg1.false, arg2.false);
			ret.true   = arg2.true;
			ret.a_type = A_BOOL;
		} else if (op == OP_OR) {
			complete(arg1.false, marker);
			ret.ptr	   = NULL;
			ret.true   = concat(arg1.true, arg2.true);
			ret.false  = arg2.false;
			ret.a_type = A_BOOL;
		} else if (op == OP_XOR) {
			struct symbol_t *sym1 = newtemp(SYM_VAR, A_BOOL);
			struct symbol_t *sym2 = newtemp(SYM_VAR, A_BOOL);
			struct list_t *next1  = action_assign(sym1, arg1);
			complete(next1, marker);
			struct list_t *next2 = action_assign(sym2, arg2);
			complete(next2, nextquad);

			struct symbol_t *res = newtemp(SYM_VAR, A_BOOL);
			gencode(OP_XOR, sym1, sym2, res);
			ret.a_type = A_BOOL;
			ret.true   = crelist(nextquad);
			gencode(OP_EQUALS, res, newtemp(SYM_CST, A_INT, 1), NULL);
			ret.false = crelist(nextquad);
			gencode(OP_GOTO, NULL);
		}
	}
	return ret;
}
