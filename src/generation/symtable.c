#include "generation/symtable.h"
#include "logger.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symtable_t *st;

struct node_t *cur_block	  = NULL;
unsigned int block_stack_size = 0;

struct node_t *sym_stack	  = NULL;
struct node_t *sym_stack_root = NULL;

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
	st->size	 = size;
	st->usage	 = 0;
	st->list	 = NULL;
	st->next_tmp = NULL;
	return st;
}

struct symbol_t *st_get(char *key) {
	unsigned long h	   = st_hash_str(key);
	struct symbol_t *e = st->table[h];
	while (e != NULL) {
		if (!strcmp(e->name, key))
			return e;
		h++;
		if (h >= st->size)
			log_error("st hash collision / table full");
		e = st->table[h];
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

	while (sym != NULL) {
		log_debug("st conflict %s <> %s", sym->name, value->name);
		if (!strcmp(sym->name, value->name)) { // var already declared
			if (get_sym_depth(sym) ==
				block_stack_size) { // already declared in same scope
				log_syntax_error(
					"syntax error : var %s already declared in this scope",
					value->name);
				return NULL;
			} else { // Redfine var in upper scope
				value->parent = sym;
				break;
			}
		} else {
			// Tmp fix in case of collision : increment hash until we find free
			// space
			// Not really performance friendly
			hash++;
			if (hash >= st->size) {
				log_error("st full");
			}
			sym = st->table[hash];
		}
	}

	// Getting here means the key doesn't already exists

	value->depth = block_stack_size;

	// add the symbol in the list of all symbols
	st->list = node_unshift(st->list, value);

	// insert symbol in hashtable
	st->table[hash] = value;

	// increase table usage number
	st->usage++;

	// add the symbol in the symbol stack
	if (sym_stack_root == NULL) { // if sym stack empty, create one
		sym_stack_root = node_create(value);
		sym_stack	   = sym_stack_root;
	} else { // append at the end
		sym_stack = node_append(sym_stack_root, value);
	}

	log_debug("Added entry %s (%i) in symtable at idx %i (depth: %i)",
			  value->name, value->atomic_type, hash, value->depth);

	return NULL;
}

void st_unshift() {
	cur_block	 = node_unshift(cur_block, sym_stack);
	st->next_tmp = node_unshift_int(st->next_tmp, 0);
	block_stack_size++;
}

struct node_t *st_shift() {
	// shift all symbols from this block
	struct symbol_t *sym;
	struct node_t *it = node_shift(&cur_block);
	sym_stack		  = it;
	it				  = it->next;

	while (it != NULL) {
		sym = it->data;

		// Replace all symbols declared in this block by its parent
		unsigned long hash = st_hash(sym);
		st->table[hash]	   = sym->parent;

		// Iterate
		it = it->next;
	}

	// remove all entries of this block in the stack
	// node_destroy(sym_stack->next, 0);
	struct node_t *ret = sym_stack->next;
	sym_stack->next	   = NULL;
	if (block_stack_size > 0) {
		block_stack_size--;
	}

	free(node_shift(&st->next_tmp));

	return ret;
}

unsigned int st_next_tmp_id() {
	unsigned int ret = *((unsigned int *)st->next_tmp->data);
	*((unsigned int *)st->next_tmp->data) += 1;
	return ret;
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
	node_destroy(cur_block, 0);
	node_destroy(sym_stack_root, 0);
	node_destroy(st->next_tmp, 1);
	free(st->table);
	free(st);
}

void st_print_scope() {
	struct symbol_t *sym;

	printf("=============== SYMBOL TABLE ===============\n");
	printf("%-9s%-8s%-15s %-5s %-15s%s\n", "SymType", "Type", "Name", "",
		   "Value", "Depth");
	for (unsigned int i = 0; i < st->size; i++) {
		sym = st->table[i];
		if (sym == NULL)
			continue;

		printf("%-9s%-8s%-15s", sym_type_str[sym->sym_type],
			   atomic_type_str[sym->atomic_type], sym->name);
		printf("=%-5s", "");
		if (sym->sym_type == SYM_CST) {
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
				break;
			}
		} else {
			printf(" %-15s", "no display");
		}
		printf("%u", get_sym_depth(sym));
		printf("\n");
	}
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
		if (sym->sym_type == SYM_CST) {
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
				break;
			}
		} else {
			printf(" %-15s", "no display");
		}
		printf("%u", get_sym_depth(sym));
		printf("\n");
		it = it->next;
	}
}
