#include "generation/defs.h"
#include "grammar.h"
#include "logger.h"
#include "util.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int nextquad		 = 0;
struct quad_t tabQuad[10000] = {0};

int gencode(uint op, ...) {
	va_list args;
	va_start(args, op);
	int index		   = nextquad;
	size_t size		   = 0;
	unsigned int label = 0;

	tabQuad[index].arg1 = NULL;
	tabQuad[index].arg2 = NULL;

	switch (op) {
	case OP_GOTO:
		label = 1;
		size  = 0;
		break;
	case OP_WRITE:
	case OP_READ:
		size = 0;
		break;
	case OP_ASSIGNMENT:
		size = 1;
		break;
	case OP_IF:
		op	  = va_arg(args, int);
		size  = 2;
		label = 1;
		break;
	default:
		size = 2;
		break;
	}

	log_debug("new quad %u", op);

	if (size > 2) {
		log_error("error: too many args for gencode");
	}

	if (size >= 1) {
		tabQuad[index].arg1 = va_arg(args, void *);
		if (size == 2) {
			tabQuad[index].arg2 = va_arg(args, void *);
		}
	}
	if (label) {
		tabQuad[index].label = va_arg(args, void *);
		if(tabQuad[index].label != NULL) {
			tabQuad[index].label->print_label = 1;
		}
	} else {
		tabQuad[index].res = va_arg(args, void *);
	}

	tabQuad[index].op = op;
	tabQuad[index].id = index;
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
		while (list1->next != NULL)
			list1 = list1->next;

		list1->next = list2;
	}

	return res;
}

void complete(struct list_t *list, unsigned int pos) {
	if (list == NULL)
		return;

	tabQuad[pos].print_label = 1;
	
	while (list != NULL) {
		tabQuad[list->position].label = &tabQuad[pos];
		list = list->next;
	}
}
