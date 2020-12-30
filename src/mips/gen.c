#include "generation/defs.h"
#include "mips/defs.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>

static unsigned int current_arg_pos = 0;

static void gen_var_decl(FILE *out, struct symbol_t *sym) {
	switch (sym->atomic_type) {
	case A_INT:
	case A_BOOL:
		mips(out, SYM, sym, COLON, TAB, RAW, ".word", TAB, IMM,
			 sym->sym_type == SYM_CST ? sym->int_val : 0, END);
		break;
	case A_STR:
		mips(out, SYM, sym, COLON, TAB, RAW, ".asciiz", TAB, IMM_STR,
			 sym->str_val, END);
		break;

	default:
		break;
	}
}

void gen_st(FILE *out) {
	// Segment data
	mips(out, TAB, RAW, ".data", END);
	struct symbol_t *tmp;
	struct node_t *it = st->list;

	while (it != NULL) {
		tmp = it->data;
		if (tmp != NULL) {
			if (tmp->sym_type == SYM_VAR) {
				gen_var_decl(out, tmp);
			} else if (tmp->sym_type == SYM_CST) {
				if (tmp->atomic_type == A_STR) {
					gen_var_decl(out, tmp);
				}
			} else if (tmp->sym_type == SYM_FUN) {
				struct node_t *it = tmp->fun_desc->par_sym_list;
				struct symbol_t *s;
				while (it != NULL) {
					s = it->data;
					if (s != NULL) {
						gen_var_decl(out, s);
					}

					it = it->next;
				}
			}
		}
		it = it->next;
	}
	mips(out, END);
}

void gen_syscall(FILE *out, enum syscall_svc_t id) {
	mips(out, LI, REG, "v0", IMM, id, END);
	mips(out, SYSCALL, END);
}

void gen_write(FILE *out, struct quad_t *quad) {
	mips(out, LOAD, REG, "a0", SYM, quad->res, END);
	switch (quad->res->atomic_type) {
	case A_INT:
	case A_BOOL:
		gen_syscall(out, SYS_PRINT_INT);
		break;
	case A_STR:
		gen_syscall(out, SYS_PRINT_STR);
		break;
	default:
		break;
	}
}

void gen_assign(FILE *out, struct quad_t *quad) {
	mips(out, LOAD, REG, "t0", SYM, quad->arg1, END);
	mips(out, SW, REG, "t0", SYM, quad->res, END);
}

void gen_call(FILE *out, struct quad_t *quad) {
	current_arg_pos = 0;

	mips(out, JAL, QLABEL, quad->label, END);
}

/**
 * @brief Stack sym value
 *
 * @param out
 * @param sym
 */
void gen_push(FILE *out, struct symbol_t *sym) {
	mips(out, LOAD, REG, "t0", SYM, sym, END);
	mips(out, INSTR_SUB, REG, "sp", REG, "sp", IMM, 4, END);
	mips(out, SW, REG, "t0", ADDR, "$sp", END);
}

void gen_push_arg(FILE *out, struct quad_t *quad) {
	if (current_arg_pos < 4) {
		char *reg = current_arg_pos == 0
						? "a0"
						: current_arg_pos == 1
							  ? "a1"
							  : current_arg_pos == 2 ? "a2" : "a3";
		mips(out, LOAD, REG, reg, SYM, quad->res, END);
	} else {
	}
	current_arg_pos++;
}

void gen_pop(FILE *out, struct symbol_t *sym) {
	mips(out, LW, REG, "t0", ADDR, "$sp", END);
	mips(out, SW, REG, "t0", SYM, sym, END);
	mips(out, INSTR_ADDI, REG, "sp", REG, "sp", IMM, 4, END);
}

void gen_pop_arg(FILE *out, struct quad_t *quad) {
	unsigned int i;
	struct node_t *it = quad->res->fun_desc->par_sym_list;
	struct symbol_t *s;

	for (i = 0; i < quad->res->fun_desc->par_nb; i++) {
		s		  = it->data;
		char *reg = i == 0 ? "a0" : i == 1 ? "a1" : i == 2 ? "a2" : "a3";
		if (i < 4) {
			mips(out, SW, REG, reg, SYM, s, END);
		} else {
			gen_pop(out, s);
		}
		it = it->next;
	}
}

void gen_quad(FILE *out, struct quad_t *quad) {
	if (quad->print_label) {
		mips(out, TAB, QLABEL, quad, COLON, END);
	}

	switch (quad->op) {
	case OP_ASSIGNMENT:
		gen_assign(out, quad);
		break;
	case OP_WRITE:
		gen_write(out, quad);
		break;
	case OP_GOTO:
		mips(out, BRANCH, QLABEL, quad->label, END);
		break;
	case OP_EQUALS:
		mips(out, LOAD, REG, "t0", SYM, quad->arg1, END);
		mips(out, LOAD, REG, "t1", SYM, quad->arg2, END);
		mips(out, BEQ, REG, "t0", REG, "t1", QLABEL, quad->label, END);
		break;
	case OP_NEGATE:
		mips(out, LOAD, REG, "t0", SYM, quad->arg1, END);
		mips(out, INSTR_MULT, REG, "t1", REG, "t0", IMM, -1, END);
		mips(out, SW, REG, "t1", SYM, quad->res, END);
		break;
	case OP_LOWER:
		mips(out, LOAD, REG, "t0", SYM, quad->arg1, END);
		mips(out, LOAD, REG, "t1", SYM, quad->arg2, END);
		mips(out, BLT, REG, "t0", REG, "t1", QLABEL, quad->label, END);
		break;
	case OP_LOWER_OR_EQUAL:
		mips(out, LOAD, REG, "t0", SYM, quad->arg1, END);
		mips(out, LOAD, REG, "t1", SYM, quad->arg2, END);
		mips(out, BLE, REG, "t0", REG, "t1", QLABEL, quad->label, END);
		break;
	case OP_SUPERIOR:
		mips(out, LOAD, REG, "t0", SYM, quad->arg1, END);
		mips(out, LOAD, REG, "t1", SYM, quad->arg2, END);
		mips(out, BGT, REG, "t0", REG, "t1", QLABEL, quad->label, END);
		break;
	case OP_SUPERIOR_OR_EQUAL:
		mips(out, LOAD, REG, "t0", SYM, quad->arg1, END);
		mips(out, LOAD, REG, "t1", SYM, quad->arg2, END);
		mips(out, BGE, REG, "t0", REG, "t1", QLABEL, quad->label, END);
		break;
	case OP_DIFFERENT:
		mips(out, LOAD, REG, "t0", SYM, quad->arg1, END);
		mips(out, LOAD, REG, "t1", SYM, quad->arg2, END);
		mips(out, BNE, REG, "t0", REG, "t1", QLABEL, quad->label, END);
		break;
	case OP_PUSH_ARG:
		gen_push_arg(out, quad);
		break;
	case OP_CALL:
		gen_call(out, quad);
		break;
	case OP_RETURN:
		mips(out, JR, REG, "ra", END);
		break;
	case OP_POP_ARG:
		gen_pop_arg(out, quad);
		break;
	case OP_ADD:
		mips(out,LOAD,REG,"t0",SYM,quad->arg1,END);
		mips(out,LOAD,REG,"t1",SYM,quad->arg2,END);
		mips(out,INSTR_ADD,REG,"t2",REG,"t0",REG,"t1",END);
		mips(out, SW, REG, "t2", SYM, quad->res, END);
		break;
	case OP_MINUS:
		mips(out,LOAD,REG,"t0",SYM,quad->arg1,END);
		mips(out,LOAD,REG,"t1",SYM,quad->arg2,END);
		mips(out,INSTR_SUB,REG,"t2",REG,"t0",REG,"t1",END);
		mips(out, SW, REG, "t2", SYM, quad->res, END);
		break;
	case OP_MULTIPLIES :
		mips(out,LOAD,REG,"t0",SYM,quad->arg1,END);
		mips(out,LOAD,REG,"t1",SYM,quad->arg2,END);
		mips(out,INSTR_MULT,REG,"t2",REG,"t0",REG,"t1",END);
		mips(out, SW, REG, "t2", SYM, quad->res, END);
		break;
	case OP_DIVIDES :
		mips(out,LOAD,REG,"t0",SYM,quad->arg1,END);
		mips(out,LOAD,REG,"t1",SYM,quad->arg2,END);
		mips(out,INSTR_DIV,REG,"t2",REG,"t0",REG,"t1",END);
		mips(out, SW, REG, "t2", SYM, quad->res, END);
		break;
	case OP_AND:
		mips(out,LOAD,REG,"t0",SYM,quad->arg1,END);
		mips(out,LOAD,REG,"t1",SYM,quad->arg2,END);
		mips(out,INSTR_AND,REG,"t2",REG,"t0",REG,"t1",END);
		mips(out, SW, REG, "t2", SYM, quad->res, END);
		break;
	case OP_OR:
		mips(out,LOAD,REG,"t0",SYM,quad->arg1,END);
		mips(out,LOAD,REG,"t1",SYM,quad->arg2,END);
		mips(out,INSTR_OR,REG,"t2",REG,"t0",REG,"t1",END);
		mips(out, SW, REG, "t2", SYM, quad->res, END);
		break;
	default:
		break;
	}
	mips(out, END);
}

void gen_exit(FILE *out) {
	fprintf(out, "\n");
	mips(out, TAB, RAW, "exit", COLON, END);
	gen_syscall(out, SYS_EXIT);
}

void gen_mips(FILE *out) {
	gen_st(out);
	unsigned int i;
	fprintf(out, "\t.text\n");
	fprintf(out, "\t.globl main\n");
	for (i = 0; i < nextquad; i++) {
		gen_quad(out, &tabQuad[i]);
	}

	gen_exit(out);
}
