#ifndef GEN_SYMBOL_H
#define GEN_SYMBOL_H

#define SYM_NAME_MAX_LEN 64

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
enum atomic_type_t { A_UNIT, A_INT, A_BOOL };

struct symbol_t {
	char name[SYM_NAME_MAX_LEN];
	enum sym_type_t sym_type;
	enum atomic_type_t atomic_type;
	int data;
};

struct symbol_t *sym_create(char *name, enum sym_type_t, enum atomic_type_t);
void sym_destroy(struct symbol_t *);

#endif