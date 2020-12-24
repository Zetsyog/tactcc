#ifndef GEN_DEF_H
#define GEN_DEF_H

#include "generation/quad.h"

typedef unsigned int uint;

int gencode(uint op, ...);
struct list_t *crelist(int pos);
struct list_t *concat(struct list_t *list1, struct list_t *list2);
void newtemp();
void complete(int *list, int addr);

#endif