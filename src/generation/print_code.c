#include "generation/print_code.h"
#include "generation/defs.h"
#include "grammar.h"
#include <stdio.h>

static void print_sym_val(struct symbol_t *sym) {
	if(sym->sym_type == SYM_VAR) {
		printf("%s", sym->name);
	} else if(sym->sym_type == SYM_CST) {
		printf("%i", sym->data);
	}
}

void print_intermediate_code() {
	unsigned int i;
	for (i = 0; i < nextquad; i++) {
		printf("%i\t", i + 1);
		struct quad_t quad = tabQuad[i];

		switch (quad.op) {
		case OP_ASSIGNMENT:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf("\n");
			break;
		case OP_ADD:
			break;
		case OP_MINUS:
			break;
		case OP_MULTIPLIES:
			break;
		case OP_DIVIDES:
			break;
		case OP_POWER:
			break;
		case OP_LOWER:
			break;
		case OP_LOWER_OR_EQUAL:
			break;
		case OP_SUPERIOR:
			break;
		case OP_SUPERIOR_OR_EQUAL:
			break;
		case OP_EQUALS:
			break;
		case OP_DIFFERENT:
			break;
		case OP_GOTO:
			// TODO
			break;
        }
	}
}
