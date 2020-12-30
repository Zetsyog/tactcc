/**
 * @file
 */
#ifndef GEN_DEF_H
#define GEN_DEF_H

#include "generation/quad.h"
#include "generation/symtable.h"

typedef unsigned int uint;

struct expr_val_t {
	enum atomic_type_t a_type;
	struct symbol_t *ptr;
	struct list_t * true;
	struct list_t * false;
};

struct expr_val_t *expr_clone(struct expr_val_t expr);

int gencode(uint op, ...);

void action_call(struct symbol_t *func, struct node_t *expr_list);
struct symbol_t *action_eval_par(struct expr_val_t expr);
struct list_t *action_assign(struct symbol_t *sym, struct expr_val_t expr);
struct expr_val_t action_opb(struct expr_val_t arg1, enum operation_t op,
							struct expr_val_t arg2, unsigned int marker);

struct list_t *crelist(int pos);
struct list_t *concat(struct list_t *list1, struct list_t *list2);
struct symbol_t *newtemp(enum sym_type_t, enum atomic_type_t, ...);
void complete(struct list_t *list, unsigned int pos);
void destroy_list(struct list_t *list);

#endif
