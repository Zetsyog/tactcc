#include "generation/defs.h"
#include "mips/defs.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>

// 		mips(out, SW, REG, "t0", SYM, quad->res, END);
#define INSTR_TO_STR_IDX(instr) (instr - FIRST_INSTR_IDX)

static const char *op_str[SYSCALL] = {[INSTR_TO_STR_IDX(LI)]		= "li",
									  [INSTR_TO_STR_IDX(LW)]		= "lw",
									  [INSTR_TO_STR_IDX(LA)]		= "la",
									  [INSTR_TO_STR_IDX(SW)]		= "sw",
									  [INSTR_TO_STR_IDX(MOVE)]		= "move",
									  [INSTR_TO_STR_IDX(SYSCALL)]	= "syscall",
									  [INSTR_TO_STR_IDX(INSTR_ADD)] = "add",
									  [INSTR_TO_STR_IDX(INSTR_ADDI)] = "addi",
									  [INSTR_TO_STR_IDX(INSTR_SUB)]	 = "sub",
									  [INSTR_TO_STR_IDX(INSTR_AND)]	 = "and",
									  [INSTR_TO_STR_IDX(INSTR_ANDI)] = "andi",
									  [INSTR_TO_STR_IDX(INSTR_NOR)]	 = "nor",
									  [INSTR_TO_STR_IDX(INSTR_OR)]	 = "or",
									  [INSTR_TO_STR_IDX(INSTR_ORI)]	 = "ori",
									  [INSTR_TO_STR_IDX(INSTR_XOR)]	 = "ori",
									  [INSTR_TO_STR_IDX(INSTR_XORI)] = "xori",
									  [INSTR_TO_STR_IDX(INSTR_DIV)]	 = "div",
									  [INSTR_TO_STR_IDX(INSTR_MULT)] = "mult",
									  [INSTR_TO_STR_IDX(BRANCH)]	 = "b",
									  [INSTR_TO_STR_IDX(BEQ)]		 = "beq"};

static void print_sym_name(FILE *out, struct symbol_t *sym) {
	if (sym->sym_type == SYM_VAR) {
		fprintf(out, VAR_PREFIX);
	} else if (sym->sym_type == SYM_FUN) {
		// TODO
	} else if (sym->sym_type == SYM_ARRAY) {
		// TODO
	}
	fprintf(out, "%s", sym->name);
}

static void print_quad_label(FILE *out, struct quad_t *quad) {
	fprintf(out, "%s_%u",
			quad->op == OP_ASSIGNMENT
				? ASSIGN_LABEL_PREFIX
				: IS_BOOL_OP(quad->op) ? LOOP_LABEL_PREFIX
									   : DEFAULT_LABEL_PREFIX,
			quad->id);
}

static void print_load(FILE *out, int reg_opt, char *reg, int arg_type,
					   struct symbol_t *arg) {
	if (reg_opt != REG) {
		return;
	}
	if (arg_type != SYM) {
		return;
	}

	if (arg->atomic_type == A_STR) {
		fprintf(out, "la ");
	} else if (arg->sym_type == SYM_CST) {
		fprintf(out, "li ");
	} else if (arg->sym_type == SYM_VAR) {
		fprintf(out, "lw ");
	}
	fprintf(out, "$%s ", reg);
	if (arg->atomic_type == A_STR) {
		print_sym_name(out, arg);
	} else if (arg->sym_type == SYM_CST) {
		fprintf(out, "%i", arg->int_val);
	} else if (arg->sym_type == SYM_VAR) {
		print_sym_name(out, arg);
	}
}

void mips(FILE *out, ...) {
	va_list args;
	va_start(args, out);

	unsigned int op, nospace = 1;
	while ((op = va_arg(args, int)) != END) {
		if (nospace) {
			nospace = 0;
		} else {
			fprintf(out, " ");
		}
		if (op == LOAD) {
			unsigned int t1		 = va_arg(args, int);
			char *reg			 = va_arg(args, char *);
			unsigned int t2		 = va_arg(args, int);
			struct symbol_t *sym = va_arg(args, void *);
			print_load(out, t1, reg, t2, sym);
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
		case IMM_STR:
			fprintf(out, "\"%s\"", va_arg(args, char *));
			break;
		case QLABEL:
			print_quad_label(out, va_arg(args, void *));
			break;
		case TAB:
			fprintf(out, "\t");
			nospace = 1;
			break;
		case RAW:
			fprintf(out, "%s", va_arg(args, char *));
			break;
		case COLON:
			fprintf(out, ":");
			nospace = 1;
			break;
		case LOAD:
			break;
		default:
			fprintf(out, "%s", op_str[INSTR_TO_STR_IDX(op)]);
			break;
		}
	}
	va_end(args);

	fprintf(out, "\n");
}