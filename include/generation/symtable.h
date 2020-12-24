#ifndef UTIL_HASHTABLE_H
#define UTIL_HASHTABLE_H

#include "generation/symbol.h"

struct st_entry_t {
	struct symbol_t *value;
	struct st_entry_t *next;
	char key[];
};

struct symtable_t {
	unsigned int size; // current size of symtable
	unsigned int usage; // number of elements in the table
	struct st_entry_t *list;
	struct st_entry_t **table; // the table where entries are stored
};

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

void st_debug();

/**
 * Store a value in the table with the given key
 * If data is already store with the same key it will be replaced
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