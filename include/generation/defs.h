#ifndef GEN_DEF_H
#define GEN_DEF_H

#include "generation/quad.h"
#include "generation/symtable.h"

typedef unsigned int uint;

int gencode(uint op, ...);
struct list_t *crelist(int pos);
struct list_t *concat(struct list_t *list1, struct list_t *list2);
struct symbol_t *newtemp(enum sym_type_t, enum atomic_type_t, ...);
void complete(struct list_t *list, void *addr);

#endif
