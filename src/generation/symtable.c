#include "generation/symtable.h"
#include "logger.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct symtable_t *st;


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
	MCHECK(st = malloc(sizeof(struct symtable_t)));
	if ((st->table = calloc(size, sizeof(struct st_entry_t *))) == NULL) {
		free(st);
		return NULL;
	}
	st->size  = size;
	st->usage = 0;
	return st;
}

struct st_entry_t *st_get(char *key) {
	unsigned long h		 = st_hash(key) % st->size;
	struct st_entry_t *e = st->table[h];
	while (e != NULL) {
		if (!strcmp(e->key, key))
			return e;
		e = e->next;
	}

	return NULL;
}

struct st_entry_t *st_put(char *key,
						  struct symbol_t *value) {
	if (st == NULL)
		return NULL;

	log_debug("Adding entry %s (%u) in symtable", value->name, value->atomic_type);

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
	size_t len = strlen(key);
	MCHECK(entry = calloc(1, sizeof(struct st_entry_t) + len + 1));
	strncpy(entry->key, key, len);
	entry->value = value;

	// Add the element at the beginning of the linked list
	entry->next		= st->list;
	st->list		= entry;
	st->table[hash] = entry;
	st->usage++;

	return NULL;
}

void st_destroy() {
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

void st_debug() {
	unsigned int idx = 0;
		while (idx < st->size) {
			if (st->table[idx] != NULL) {
				printf("%s\n", st->table[idx]->value->name);
			}
			idx++;
		}
}