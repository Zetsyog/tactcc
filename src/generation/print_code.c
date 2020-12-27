#include "generation/print_code.h"
#include "generation/defs.h"
#include "grammar.h"
#include <stdio.h>

static void print_sym_val(struct symbol_t *sym) {
	if (sym->sym_type == SYM_VAR) {
		printf("%s", sym->name);
	} else if (sym->sym_type == SYM_CST) {
		switch (sym->atomic_type) {
		case A_INT:
			printf("%i", sym->int_val);
			break;
		case A_STR:
			printf("\"%s\"", sym->str_val);
		default:
			break;
		}
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
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" + ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_MINUS:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" - ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_MULTIPLIES:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" * ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_DIVIDES:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" / ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_POWER:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" ^ ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_LOWER:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" < ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_LOWER_OR_EQUAL:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" <= ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_SUPERIOR:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" > ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_SUPERIOR_OR_EQUAL:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" >= ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_EQUALS:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" = ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_DIFFERENT:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" <> ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_GOTO:
			if (quad.arg1 == NULL) {
				printf("GOTO ?");
			} else {
				printf(" GOTO ");
				print_sym_val(quad.res);
				printf("\n");
			}

			// TODO

			break;
		case OP_NEGATE:
			print_sym_val(quad.res);
			printf(" := ");
			printf(" - ");
			print_sym_val(quad.arg1);
			printf("\n");
			break;
		case OP_AND:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" AND ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_OR:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" OR ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_XOR:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" XOR ");
			print_sym_val(quad.arg2);
			printf("\n");
			break;
		case OP_NOT:
			print_sym_val(quad.res);
			printf(" := ");
			printf(" NOT ");
			print_sym_val(quad.arg1);
			printf("\n");
			break;
		case OP_WRITE:
			printf("%s ", "write");
			print_sym_val(quad.res);
			printf("\n");
			break;
		case OP_READ:
			printf("%s ", "write");
			print_sym_val(quad.res);
			printf("\n");
			break;
		default:
			break;
		}
	}
}
