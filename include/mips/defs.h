#ifndef MIPS_DEFS_H
#define MIPS_DEFS_H

#include <stdio.h>

#define VAR_PREFIX "_uvar_"
#define TMP_PREFIX "__"
#define IF_PREFIX "_if_"

enum syscall_svc_t {
	SYS_PRINT_INT	 = 1,
	SYS_PRINT_FLOAT	 = 2,
	SYS_PRINT_DOUBLE = 3,
	SYS_PRINT_STR	 = 4,
	SYS_READ_INT	 = 5,
	SYS_READ_FLOAT	 = 6,
	SYS_READ_DOUBLE	 = 7,
	SYS_READ_STR	 = 8,
	SYS_SBRK		 = 9,
	SYS_EXIT		 = 10,
	SYS_PRINT_CHAR	 = 11,
	SYS_READ_CHAR	 = 12,
	SYS_OPEN		 = 13,
	SYS_READ		 = 14,
	SYS_WRITE		 = 15,
	SYS_CLOSE		 = 16,
	SYS_EXIT2		 = 17
};

enum mips_helper_op_t {
	END = 0,
	// Data Formats
	IMM = 1,
	SYM = 2,
	REG = 3,
	IMM_STR = 4,
	// TODO
	// Instructions :
	LI		= 10,
	LW		= 11,
	LA		= 12,
	SW		= 13,
	MOVE	= 14,
	INSTR_ADD   = 15,
	INSTR_ADDI  = 16,
	INSTR_SUB   = 17,
	INSTR_AND   = 18,
	INSTR_ANDI  = 19,
	INSTR_NOR   = 20,
	INSTR_OR    = 21,
	INSTR_ORI 	= 22,
	INSTR_XOR 	= 23,
	INSTR_XORI	= 24,
	INSTR_DIV		= 25,
	INSTR_MULT 	= 26,
	SYSCALL = 100,
    RAW = 101,
    TAB = 102,
    DECL = 103,

};

#define FIRST_INSTR_IDX LI
#define LAST_INSTR_IDX SYSCALL - LI


void mips(FILE *out, ...);

void gen_st(FILE *out);
void gen_mips(FILE *out);

#endif
