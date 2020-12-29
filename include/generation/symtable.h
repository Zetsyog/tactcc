/**
 * @file
 */
#ifndef GEN_SYMTABLE_H
#define GEN_SYMTABLE_H

#include "generation/symbol.h"
#include "util.h"

#define TMP_VAR_PREFIX "__tmp__"
#define CONST_PREFIX "__const__"

/**
 * Default size for a new symtable
 */
#define DEFAULT_ST_SIZE 10000

/**
 * @struct
 * A struct representing a table of symbol
 */
struct symtable_t {
	/**
	 * Maximum size of symtable
	 */
	unsigned int size;
	/**
	 * Number of elements in the table
	 */
	unsigned int usage; 
	/**
	 * Pointer to the last entry
	 */
	struct node_t *list;
	/**
	 * Table of size symtable_t::size \n
	 * Store all the registred symbols
	 */
	struct symbol_t **table; 
};

/**
 * The main table of symbol
 */
extern struct symtable_t *st;

extern struct node_t *cur_block;

extern struct node_t *sym_stack;

/**
 * Create a new symbol table
 * @param size the initial size of the table
 * @return the created hashtable
 */
struct symtable_t *st_create(unsigned int size);

/**
 * Retrieve data for a given key
 * @param t the symtable to search in
 * @param key the key to search for
 * @return the data looked for, else return NULL
 */
struct symbol_t *st_get(char *key);

/**
 * Stack a new local table of symbol
 */
void st_unshift();

/**
 * Pop the current local table of symbol
 */
void st_shift();

/**
 * Print the symbol table in stdout
 */
void st_print();

/**
 * Store a value in the table with the given key \n
 * If data is already store with the same key it will be replaced \n
 * TODO : check if it is not better to use symbol_t * as value
 * @param key
 * @param value
 * @return NULL on error
 */
struct symbol_t *st_put(struct symbol_t *value);

// TODO: maybe add new functions like clear, remove, iterate
// dunno if needed

/**
 * Destroy a symbol table
 */
void st_destroy();

#endif