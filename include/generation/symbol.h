/**
 * @file
 */
#ifndef GEN_SYMBOL_H
#define GEN_SYMBOL_H

#define SYM_NAME_MAX_LEN 64

/**
 * Type of a symbol
 */
enum sym_type_t {
	SYM_CST	  = 0, /* Constant */
	SYM_VAR	  = 1, /* Variable */
	SYM_ARRAY = 2, /* Array */
	SYM_FUN	  = 3  /* Function */
};

extern char *sym_type_str[];

/**
 * Atomic types availables in Scalpa
 */
// TODO: move in a more appropriate location
enum atomic_type_t { A_UNIT = 0, A_INT = 1, A_BOOL = 2, A_STR = 3 };
extern char *atomic_type_str[];

struct symbol_t {
	char name[SYM_NAME_MAX_LEN];
	enum sym_type_t sym_type;
	enum atomic_type_t atomic_type;
	union {
		int int_val;
		char *str_val;
	};
};

/**
 * Create and allocate a new symbole_t
 * @param name of the symbol
 * @param sym_type @see sym_type_t
 * @param a_type atomic type @ee atomic_type_t
 * @return pointer to the created symbol
 */
struct symbol_t *sym_create(char *name, enum sym_type_t sym_type, enum atomic_type_t a_type);

/**
 * Free memory for a given symbol
 * @param sym The symbol to destroy
 */
void sym_destroy(struct symbol_t *sym);

#endif