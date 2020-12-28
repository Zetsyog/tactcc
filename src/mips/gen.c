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
		if (st->table[i] != NULL && st->table[i]->value != NULL) {
			tmp = st->table[i]->value;
			switch (tmp->atomic_type) {
			case A_INT:
			case A_BOOL:
				mips(out, DECL, tmp, TAB, RAW, ".word", TAB, IMM, tmp->int_val,
					 END);
				break;
			case A_STR:
				mips(out, DECL, tmp, TAB, RAW, ".asciiz", TAB, IMM_STR,
					 tmp->str_val, END);
				break;

			default:
				break;
			}
		}
		i++;
	}
}

void gen_syscall(FILE *out, enum syscall_svc_t id) {
	mips(out, LI, REG, "v0", IMM, id, END);
	mips(out, SYSCALL, END);
}

void gen_quad(FILE *out, struct quad_t *quad) {
	if (quad->print_label)
		mips(out, TAB, QLABEL, quad, COLON, END);

	if (quad->op == OP_ASSIGNMENT) {
		mips(out, LW, REG, "t0", SYM, quad->arg1, END);
		mips(out, SW, REG, "t0", SYM, quad->res, END);
	} else if (quad->op == OP_WRITE) {
		switch (quad->res->atomic_type) {
		case A_INT:
		case A_BOOL:
			mips(out, LW, REG, "a0", SYM, quad->res, END);
			gen_syscall(out, SYS_PRINT_INT);
			break;
		case A_STR:
			mips(out, LA, REG, "a0", SYM, quad->res, END);
			gen_syscall(out, SYS_PRINT_STR);
			break;
		default:
			break;
		}
	} else if (quad->op == OP_GOTO) {
		mips(out, BRANCH, QLABEL, quad->label, END);
	}
}

void gen_exit(FILE *out) {
	fprintf(out, "\n\n");
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