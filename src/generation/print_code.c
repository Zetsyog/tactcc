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
		case A_BOOL:
			printf("%i", sym->int_val);
			break;
		case A_STR:
			printf("\"%s\"", sym->str_val);
		default:
			break;
		}
	} else if (sym->sym_type == SYM_PAR) {
		printf("%s::%s", sym->str_val, sym->name);
	}
}

void print_op_cond(unsigned int i, const char *op) {
	struct quad_t quad = tabQuad[i];
	printf("IF ");
	print_sym_val(quad.arg1);
	printf(" %s ", op);
	print_sym_val(quad.arg2);
	if (quad.label == NULL) {
		printf(" GOTO ?");
	} else {
		printf(" GOTO ");
		printf("%i", quad.label->id);
	}
}

void print_intermediate_code() {
	unsigned int i;
	for (i = 0; i < nextquad; i++) {
		printf("%i\t", i);
		struct quad_t quad = tabQuad[i];

		switch (quad.op) {
		case OP_ASSIGNMENT:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			break;
		case OP_ADD:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" + ");
			print_sym_val(quad.arg2);
			break;
		case OP_MINUS:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" - ");
			print_sym_val(quad.arg2);
			break;
		case OP_MULTIPLIES:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" * ");
			print_sym_val(quad.arg2);
			break;
		case OP_DIVIDES:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" / ");
			print_sym_val(quad.arg2);
			break;
		case OP_POWER:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" ^ ");
			print_sym_val(quad.arg2);
			break;
		case OP_LOWER:
			print_op_cond(i, "<");
			break;
		case OP_LOWER_OR_EQUAL:
			print_op_cond(i, "<=");
			break;
		case OP_SUPERIOR:
			print_op_cond(i, ">");
			break;
		case OP_SUPERIOR_OR_EQUAL:
			print_op_cond(i, ">=");
			break;
		case OP_EQUALS:
			print_op_cond(i, "==");
			break;
		case OP_DIFFERENT:
			print_op_cond(i, "<>");
			break;
		case OP_GOTO:
			if (quad.label == NULL) {
				printf("GOTO ?");
			} else {
				printf("GOTO ");
				printf("%i", quad.label->id);
			}
			break;
		case OP_NEGATE:
			print_sym_val(quad.res);
			printf(" := ");
			printf(" - ");
			print_sym_val(quad.arg1);
			break;
		case OP_AND:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" AND ");
			print_sym_val(quad.arg2);
			break;
		case OP_OR:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" OR ");
			print_sym_val(quad.arg2);
			break;
		case OP_XOR:
			print_sym_val(quad.res);
			printf(" := ");
			print_sym_val(quad.arg1);
			printf(" XOR ");
			print_sym_val(quad.arg2);
			break;
		case OP_NOT:
			print_sym_val(quad.res);
			printf(" := ");
			printf(" NOT ");
			print_sym_val(quad.arg1);
			break;
		case OP_WRITE:
			printf("%s ", "write");
			print_sym_val(quad.res);
			break;
		case OP_READ:
			printf("%s ", "read");
			print_sym_val(quad.res);
			break;
		case OP_CALL:
			printf("%s ", "call");
			printf("%i", quad.label->id);
			break;
		case OP_RETURN:
			printf("return ");
			if (quad.res != NULL) {
				print_sym_val(quad.res);
			}
			break;
		case OP_PUSH_ARG:
			printf("push_arg ");
			print_sym_val(quad.res);
			break;
		case OP_POP_ARG:
			printf("pop_arg ");
			print_sym_val(quad.res);
			break;
		case OP_PUSH_RET:
			printf("push_ret ");
			print_sym_val(quad.res);
			break;
		case OP_POP_RET:
			printf("pop_ret ");
			print_sym_val(quad.res);
			break;
		case OP_EXIT:
			printf("EXIT");
			break;
		default:
			printf("???");
			break;
		}
		if(quad.is_main) {
			printf(" <----- main");
		}
		printf("\n");
	}
}
