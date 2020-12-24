#include "generation/defs.h"
#include "grammar.h"
#include "logger.h"
#include "util.h"
#include <stdarg.h>
#include <stdlib.h>

int gencode(uint op, ...) {
	va_list args;
	va_start(args, op);
	int index	= nextquad;
	size_t size = 0;

	log_debug("new quad %u", op);

	if (op == GOTO) {
		tabQuad[index].arg1 = NULL;
		tabQuad[index].arg2 = NULL;
		tabQuad[index].res	= va_arg(args, int *);
	} else if (op == READ || op == WRITE) {
		tabQuad[index].arg1 = va_arg(args, char *);
		tabQuad[index].arg2 = NULL;
		tabQuad[index].res	= NULL;
	} else if (op == ASSIGNMENT) {
		tabQuad[index].arg1 = va_arg(args, char *);
		tabQuad[index].arg2 = NULL;
		tabQuad[index].res	= va_arg(args, int *);
		log_debug("%s := %s\n", ((struct symbol_t *)tabQuad[index].res)->name,
				  tabQuad[index].arg1);
	} else {
		tabQuad[index].arg1 = va_arg(args, void *);
		tabQuad[index].arg2 = NULL;
		if (op >= 15)
			size = 2;
		else
			size = 3;

		if (size >= 3)
			tabQuad[index].arg2 = va_arg(args, void *);

		tabQuad[index].res = NULL;

		if (size >= 2)
			tabQuad[index].res = va_arg(args, void *);
	}

	tabQuad[index].op = op;
	va_end(args);
	nextquad++;
	return index;
}

struct list_t *crelist(int pos) {
	struct list_t *l;
	MCHECK(l = malloc(sizeof(struct list_t)));
	l->position = pos;
	l->next		= NULL;
	return l;
}

struct list_t *concat(struct list_t *list1, struct list_t *list2) {
	struct list_t *res;
	if (list1 != NULL)
		res = list1;
	else if (list2 != NULL)
		res = list2;
	else
		res = NULL;

	if (list1 != NULL) {
		while (list1 != NULL)
			list1 = list1->next;

		list1->next = list2;
	}

	return res;
}
