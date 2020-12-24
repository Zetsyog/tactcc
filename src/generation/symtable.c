#include "generation/symtable.h"
#include "logger.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

/**
 * djb2 hash function by Dan Bernstein.
 */
unsigned long st_hash(char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

struct symtable_t *st_create(unsigned int size) {
	struct symtable_t *st;
	MCHECK(st = malloc(sizeof(struct symtable_t)));
	if ((st->table = calloc(size, sizeof(struct st_entry_t *))) == NULL) {
		free(st);
		return NULL;
	}
	st->size  = size;
	st->usage = 0;
	return st;
}

struct st_entry_t *st_get(struct symtable_t *st, char *key) {
	unsigned long h		 = st_hash(key) % st->size;
	struct st_entry_t *e = st->table[h];
	while (e != NULL) {
		if (!strcmp(e->key, key))
			return e;
		e = e->next;
	}

	return NULL;
}

struct st_entry_t *st_put(struct symtable_t *st, char *key,
						  struct symbol_t value) {
	if (st == NULL)
		return NULL;

	unsigned long hash		 = st_hash(key) % st->size;
	struct st_entry_t *entry = st->table[hash];

	while (entry != NULL) {
		if (!strcmp(entry->key, key)) {
			entry->value = value;
			return entry;
		}
		entry = entry->next;
	}

	// Getting here means the key doesn't already exist
	MCHECK(entry = malloc(sizeof(struct st_entry_t)) + strlen(key) + 1);
	strcpy(entry->key, key);
	entry->value = value;

	// Add the element at the beginning of the linked list
	entry->next		= st->list;
	st->list		= entry;
	st->table[hash] = entry;
	st->usage++;

	return NULL;
}

void st_destroy(struct symtable_t *st) {
	if (st->usage != 0) {
		unsigned int idx = 0;
		while (idx < st->size) {
			if (st->table[idx] != NULL) {
				free(st->table[idx]);
				st->table[idx] = NULL;
			}
			idx++;
		}
	}
	free(st->table);
	free(st);
}