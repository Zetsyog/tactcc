#include "generation/defs.h"
#include "mips/defs.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>

void gen_st(FILE *out) {
	// Segment data
	mips(out, TAB, RAW, ".data", END);
	unsigned int i = 0;
	struct symbol_t *tmp;

	while (i < st->size) {
		if (st->table[i] != NULL && st->table[i] != NULL) {
			tmp = st->table[i];
			if (tmp->sym_type == SYM_VAR) {
				switch (tmp->atomic_type) {
				case A_INT:
				case A_BOOL:
					mips(out, SYM, tmp, COLON, TAB, RAW, ".word", TAB, IMM,
						 tmp->int_val, END);
					break;
				case A_STR:
					mips(out, SYM, tmp, COLON, TAB, RAW, ".asciiz", TAB,
						 IMM_STR, tmp->str_val, END);
					break;

				default:
					break;
				}
			} else if (tmp->sym_type == SYM_CST) {
				if (tmp->atomic_type == A_STR) {
					mips(out, SYM, tmp, COLON, TAB, RAW, ".asciiz", TAB,
						 IMM_STR, tmp->str_val, END);
				}
			}
		}
		i++;
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
	fprintf(out, "\tmain:\n");
	for (i = 0; i < nextquad; i++) {
		gen_quad(out, &tabQuad[i]);
	}

	gen_exit(out);
}