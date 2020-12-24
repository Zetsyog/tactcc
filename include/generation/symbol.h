#ifndef GEN_SYMBOL_H
#define GEN_SYMBOL_H

/**
 * Type of a symbol
 */
enum sym_type_t {
	SYM_CST,   /* Constant */
	SYM_VAR,   /* Variable */
	SYM_ARRAY, /* Array */
	SYM_FUN	   /* Function */
};

/**
 * Atomic types availables in Scalpa
 */
// TODO: move in a more appropriate location
enum atomic_type_t { UNIT, INT, ARRAY };

struct symbol_t {
	enum sym_type_t sym_type;
	enum atomic_type_t atomic_type;
};

#endif