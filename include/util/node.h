/**
 * @file
 */
#ifndef UTIL_LINKED_LIST_H
#define UTIL_LINKED_LIST_H

#include "util.h"

/**
 * A linked list with a pointer value.
 * Used for var declaration list.
 */
struct node_t {
	void *data;
	struct node_t *next;
};

struct node_t *node_create(void *data);
struct node_t *node_create_int(int value);
struct node_t *node_append(struct node_t *list, void *data);
struct node_t *node_append_int(struct node_t *list, int data);
void node_destroy(struct node_t *node, int free_mem);

#endif