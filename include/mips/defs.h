/**
 * @file
 */
#ifndef MIPS_DEFS_H
#define MIPS_DEFS_H

#include <stdio.h>

#define VAR_PREFIX "_uvar_"
#define TMP_PREFIX "__"
#define LOOP_LABEL_PREFIX "__loop"
#define ASSIGN_LABEL_PREFIX "__assign"
#define DEFAULT_LABEL_PREFIX "__label"
#define PAR_PREFIX "__par"

/**
 * Syscall mips constants
 * See MIPS doc to check arg types and in which register they must be stored
 */
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
	IMM = 1,	 // Print the next arg as an int immediate value
	SYM = 2,	 // Next arg must be a pointer to a symbol_t
				 // Print its value if its a const, or its name if its a var
	REG = 3,	 // Next arg must be a string refering to a register
				 // It will be printed with register format (with a '$' before)
	IMM_STR = 4, // Print next arg as an immediate string (surrounded by '"')
	QLABEL	= 5, // Next arg must be a pointer to a quad
				 // Generate and print the quad label
	ADDR = 6,
	// Instructions :
	// Following args simply print the correspoding mips instruction
	// no args are needed
	LI		   = 10,
	LW		   = 11,
	LA		   = 12,
	SW		   = 13,
	MOVE	   = 14,
	INSTR_ADD  = 15,
	INSTR_ADDI = 16,
	INSTR_SUB  = 17,
	INSTR_AND  = 18,
	INSTR_ANDI = 19,
	INSTR_NOR  = 20,
	INSTR_OR   = 21,
	INSTR_ORI  = 22,
	INSTR_XOR  = 23,
	INSTR_XORI = 24,
	INSTR_DIV  = 25,
	INSTR_MULT = 26,
	BRANCH	   = 27,
	BEQ		   = 28,
	JAL		   = 29,
	JR		   = 30,
	BLT		   = 31,
	BGT		   = 32,
	BLE		   = 33,
	BGE		   = 34,
	BNE		   = 35, 
	// Char
	TAB	  = 100, // Print tab char
	COLON = 101, // Print colon char
	// Other
	SYSCALL = 110, // Print syscall op
	RAW		= 111, // Print next arg as raw string
	LOAD	= 112

};

#define FIRST_INSTR_IDX LI
#define LAST_INSTR_IDX SYSCALL - LI

/**
 * @brief A helper function to generate mips assembly in a file
 * The last param MUST be END
 * @see mips_helper_op_t
 *
 * @param out the file to print to
 */
void mips(FILE *out, ...);

/**
 * @brief Get the name of a symbol
 *
 * @param dest The string where to write the name
 * @param sym
 */
void get_sym_name(char *dest, struct symbol_t *sym);

/**
 * Generate mips code for the symbol table
 * @param out
 */
void gen_st(FILE *out);

/**
 * Generate mips code from all the generated quads
 * and write it in FILE *out
 * @param out
 */
void gen_mips(FILE *out);

#endif
