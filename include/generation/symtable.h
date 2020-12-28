/**
 * @file
 */
#ifndef GEN_SYMTABLE_H
#define GEN_SYMTABLE_H

#include "generation/symbol.h"

#define TMP_VAR_PREFIX "__tmp__"
#define CONST_PREFIX "__const__"

/**
 * Default size for a new symtable
 */
#define DEFAULT_ST_SIZE 10000

/**
 * A symtable entry
 */
struct st_entry_t {
	/**
	 * Value of the entry, a pointer to a symbolt
	 * @see symbol_t
	 */
	struct symbol_t *value;
	/**
	 * Pointer to the next entry of the table
	 */
	struct st_entry_t *next;
	/**
	 * A string containing the name of the entry
	 */
	char key[];
};

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
	struct st_entry_t *list;
	/**
	 * Table of size symtable_t::size \n
	 * Store all the registred symbols
	 */
	struct st_entry_t **table; 
};

/**
 * The main table of symbol
 */
extern struct symtable_t *st;

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
struct st_entry_t *st_get(char *key);

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
 * @return NULL on success
 */
struct st_entry_t *st_put(char *key,
						  struct symbol_t *value);

// TODO: maybe add new functions like clear, remove, iterate
// dunno if needed

/**
 * Destroy a symbol table
 */
void st_destroy();

#endif