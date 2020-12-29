#include "generation/symtable.h"
#include "logger.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symtable_t *st;

struct node_t *cur_block	  = NULL;
unsigned int block_stack_size = 0;

struct node_t *sym_stack = NULL;

/**
 * djb2 hash function by Dan Bernstein.
 */
unsigned long djb42(char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

unsigned long st_hash_str(char *str) {
	return djb42(str) % st->size;
}

unsigned long st_hash(struct symbol_t *sym) {
	return st_hash_str(sym->name);
}

struct symtable_t *st_create(unsigned int size) {
	MCHECK(st = calloc(1, sizeof(struct symtable_t)));
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
	unsigned long h	   = st_hash_str(key);
	struct symbol_t *e = st->table[h];
	if (e != NULL) {
		if (!strcmp(e->name, key))
			return e;
	}

	return NULL;
}

unsigned int get_sym_depth(struct symbol_t *sym) {
	return sym->depth;
}

struct symbol_t *st_put(struct symbol_t *value) {
	if (st == NULL)
		return NULL;

	unsigned long hash	 = st_hash(value);
	struct symbol_t *sym = st->table[hash];

	if (sym != NULL) {
		if (!strcmp(sym->name, value->name)) { // var already declared
			if (get_sym_depth(sym) ==
				block_stack_size) { // already declared in same scope
				log_error(
					"syntax error : var %s already declared in this scope",
					value->name);
			} else {
				value->parent = sym;
			}
		} else {
			// TODO: fix this kind of error
			log_error("symtable hash collision");
			return NULL;
		}
	}

	// Getting here means the key doesn't already exists

	// Add the element at the beginning of the linked list
	// entry->next		= st->list;
	// st->list		= entry;
	log_debug("Adding entry %s (%i) in symtable at idx %i", value->name,
			  value->atomic_type, hash);

	value->depth = block_stack_size;
	// add the symbol in the list of all symbols
	st->list		= node_unshift(st->list, value);
	st->table[hash] = value;
	st->usage++;
	// add the symbol in the symbol stack
	sym_stack = node_unshift(sym_stack, value);

	st_print();
	return NULL;
}

void st_unshift() {
	cur_block = node_unshift(cur_block, sym_stack);
	block_stack_size++;
}

void st_shift() {
	if (block_stack_size > 0) {
		block_stack_size--;
	}
	// shift all symbols from this block
	struct symbol_t *sym;
	while ((sym = node_shift(&sym_stack)) != cur_block->data && sym != NULL) {
		if (sym->parent != NULL) {
			unsigned long hash = st_hash(sym);
			st->table[hash]	   = sym->parent;
		}
	}
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
	struct node_t *it = st->list;
	struct symbol_t *sym;

	printf("=============== SYMBOL TABLE ===============\n");
	printf("%-9s%-8s%-15s %-5s %-15s%s\n", "SymType", "Type", "Name", "",
		   "Value", "Depth");
	while (it != NULL) {
		sym = it->data;
		printf("%-9s%-8s%-15s", sym_type_str[sym->sym_type],
			   atomic_type_str[sym->atomic_type], sym->name);
		printf("=%-5s", "");
		switch (sym->atomic_type) {
		case A_INT:
		case A_BOOL:
			if (sym->int_val >= 0)
				printf(" ");
			printf("%-15i", sym->int_val);
			break;
		case A_STR:
			printf(" %-15s", sym->str_val);
			break;
		default:
			printf("%-15s", "no display");
			break;
		}
		printf("%u", get_sym_depth(sym));
		printf("\n");
		it = it->next;
	}
}