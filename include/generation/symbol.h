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
	SYM_CST	  = 0, /** Constant */
	SYM_VAR	  = 1, /** Variable */
	SYM_ARRAY = 2, /** Array */
	SYM_FUN	  = 3, /** Function */
	SYM_PAR	  = 4  /** Function parameter */
};

extern char *sym_type_str[];

/**
 * Atomic types availables in Scalpa
 */
enum atomic_type_t { A_UNIT = 0, A_INT = 1, A_BOOL = 2, A_STR = 3 };
extern char *atomic_type_str[];

/**
 * Function descriptor struct
 */
struct fun_desc_t {
	unsigned int par_nb;		 /** Number of parameters */
	struct quad_t *quad;		 /** First quad of function */
	struct node_t *par_sym_list; /** List of parameters */
	struct node_t *sym_list;	 /** List of all symbols */
};

struct symbol_t {
	char name[SYM_NAME_MAX_LEN]; /** Name of the symbol */
	/**
	 * @brief pointer to the parent symbol
	 * The parent symbol is a symbol with the same name but in a upper block
	 */
	struct symbol_t *parent;
	/**
	 * @brief depth of the symbol
	 * The depth is used to
	 */
	unsigned int depth;
	enum sym_type_t sym_type;		/** Symbol type */
	enum atomic_type_t atomic_type; /** Atomic type */
	unsigned int is_tmp;
	union {
		/**
		 * @brief Int value of the symbol
		 * Used only for int constants
		 */
		int int_val;
		/**
		 * @brief String value of symbol
		 * Used for string constants and for parameters to point to the function
		 * name
		 */
		char *str_val;
		/**
		 * @brief Pointer to a function description structure
		 * Defined for SYM_FUN symboles
		 */
		struct fun_desc_t *fun_desc;
	};
};

/**
 * Create and allocate a new symbole_t
 * @param name of the symbol
 * @param sym_type @see sym_type_t
 * @param a_type atomic type @ee atomic_type_t
 * @return pointer to the created symbol
 */
struct symbol_t *sym_create(char *name, enum sym_type_t sym_type,
							enum atomic_type_t a_type);

struct fun_desc_t *fun_desc_create(unsigned int quad,
								   struct node_t *par_sym_list);

void fun_desc_destroy(struct fun_desc_t *desc);

/**
 * Free memory for a given symbol
 * @param sym The symbol to destroy
 */
void sym_destroy(struct symbol_t *sym);

#endif
