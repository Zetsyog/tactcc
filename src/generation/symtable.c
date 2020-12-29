#include "generation/symtable.h"
#include "logger.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	if ((st->table = calloc(size, sizeof(struct symbol_t *))) == NULL) {
		free(st);
		return NULL;
	}
	st->size  = size;
	st->usage = 0;
	st->list  = NULL;
	return st;
}

struct symbol_t *st_get(char *key) {
	unsigned long h	   = st_hash(key) % st->size;
	struct symbol_t *e = st->table[h];
	if (e != NULL) {
		if (!strcmp(e->name, key))
			return e;
	}

	return NULL;
}

struct symbol_t *st_put(struct symbol_t *value) {
	if (st == NULL)
		return NULL;

	unsigned long hash	 = st_hash(value->name) % st->size;
	struct symbol_t *sym = st->table[hash];

	if (sym != NULL) {
		if (!strcmp(sym->name, value->name)) { // var already declared

		} else {
			// collision error
			return NULL;
		}
	}

	// Getting here means the key doesn't already exists

	// Add the element at the beginning of the linked list
	// entry->next		= st->list;
	// st->list		= entry;
	log_debug("Adding entry %s (%u) in symtable at idx %i", value->name,
			  value->atomic_type, hash);
	st->list		= node_unshift(st->list, value);
	st->table[hash] = value;
	st->usage++;

	return NULL;
}

void st_unshift() {

}

void st_shift() {
}

void st_destroy() {
	if (st->usage != 0) {
		struct node_t *it = st->list;
		while (it != NULL) {
			sym_destroy(it->data);
			it = it->next;
		}
	}
	node_destroy(st->list, 0);
	free(st->table);
	free(st);
}

void st_print() {
	unsigned int idx = 0;
	struct symbol_t *tmp;

	printf("=============== SYMBOL TABLE ===============\n");
	printf("%-9s%-8s%-15s %-5s %s\n", "SymType", "Type", "Name", "", "Value");
	while (idx < st->size) {
		if (st->table[idx] != NULL) {
			tmp = st->table[idx];
			printf("%-9s%-8s%-15s", sym_type_str[tmp->sym_type],
				   atomic_type_str[tmp->atomic_type], tmp->name);
			printf("=%-5s", "");
			switch (tmp->atomic_type) {
			case A_INT:
			case A_BOOL:
				if (tmp->int_val >= 0)
					printf(" ");
				printf("%i", tmp->int_val);
				break;
			case A_STR:
				printf("\"%s\"", tmp->str_val);
				break;
			default:
				printf("no display");
				break;
			}
			printf("\n");
		}
		idx++;
	}
}