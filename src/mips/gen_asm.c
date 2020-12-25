#include "mips/defs.h"
#include "generation/defs.h"
#include "util.h"
#include <stdio.h>

void gen_st(FILE *out) {
    // Segment data
	fprintf(out, "\t%s\n", ".data");
	unsigned int i = 0;
    struct symbol_t *tmp;

	while (i < st->size) {
		if (st->table[i] != NULL && st->table[i]->value != NULL) {
			tmp = st->table[i]->value;
			fprintf(out, "__%s:\t%s\t%i\n", tmp->name, ".word", tmp->data);
		}
		i++;
	}
}

void gen_quad(FILE *out, struct quad_t *quad) {
    if(quad->op == OP_ASSIGNMENT) {
		fprintf(out, "\t%s %s __%s\n", "lw", "$t0", to_sym(quad->arg1)->name);
		fprintf(out ,"\t%s %s __%s\n", "sw", "$t0", to_sym(quad->res)->name);
	}
}

void gen_exit(FILE *out) {
	fprintf(out, "\n\n");
	fprintf(out, "\t%s %s %i\n", "li", "$v0", 10);
	fprintf(out, "\t%s\n", "syscall");
}

void gen_mips(FILE *out) {
	gen_st(out);
	unsigned int i;
	fprintf(out, "\t.text\n");
	fprintf(out, "\t.globl main\n");
	fprintf(out, "main:\n");
	for (i = 0; i < nextquad; i++) {
		gen_quad(out, &tabQuad[i]);
	}

	gen_exit(out);
}