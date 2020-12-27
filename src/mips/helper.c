#include "mips/defs.h"
#include "generation/defs.h"
#include <stdarg.h>
#include <stdio.h>

// 		mips(out, SW, REG, "t0", SYM, quad->res, END);
#define INSTR_TO_STR_IDX(instr) (instr - FIRST_INSTR_IDX)

static const char *op_str[SYSCALL] = {
	[INSTR_TO_STR_IDX(LI)] = "li",	   [INSTR_TO_STR_IDX(LW)] = "lw",
	[INSTR_TO_STR_IDX(LA)] = "la",	   [INSTR_TO_STR_IDX(SW)] = "sw",
	[INSTR_TO_STR_IDX(MOVE)] = "move", [INSTR_TO_STR_IDX(SYSCALL)] = "syscall"};

static void print_sym_name(FILE *out, struct symbol_t *sym) {
	if(sym->sym_type == SYM_VAR) {
		fprintf(out, VAR_PREFIX);
	} else if(sym->sym_type == SYM_FUN) {
		// TODO
	} else if(sym->sym_type == SYM_ARRAY) {
		// TODO
	}
	fprintf(out, "%s", sym->name);
}

void mips(FILE *out, ...) {
	va_list args;
	va_start(args, out);

	unsigned int op, nospace = 1;
	while ((op = va_arg(args, int)) != LR) {
		if(nospace) {
			nospace = 0;
		} else {
			fprintf(out, " ");
		}
		switch (op) {
		case IMM:
			fprintf(out, "%i", va_arg(args, int));
			break;
		case SYM:
			print_sym_name(out, va_arg(args, void *));
			break;
		case REG:
			fprintf(out, "$%s", va_arg(args, char *));
			break;
		case TAB:
			fprintf(out, "\t");
			nospace = 1;
			break;
		case RAW:
			fprintf(out, "%s", va_arg(args, char *));
			break;
		case DECL:
			print_sym_name(out, va_arg(args, void *));
			fprintf(out, ":");
			break;
		default:
			fprintf(out, "%s", op_str[INSTR_TO_STR_IDX(op)]);
			break;
		}
	}
	va_end(args);

	fprintf(out, "\n");
}