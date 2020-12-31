#include "util/node.h"
#include <stdlib.h>

struct node_t *node_create(void *data) {
	struct node_t *n;
	MCHECK(n = malloc(sizeof(struct node_t)));
	n->data = data;
	n->next = NULL;

	return n;
}

struct node_t *node_create_int(int value) {
	struct node_t *n;
	MCHECK(n = malloc(sizeof(struct node_t)));
	MCHECK(n->data = malloc(sizeof(int)));
	*((int *)n->data) = value;
	n->next			  = NULL;

	return n;
}

struct node_t *node_append(struct node_t *list, void *data) {
	if (list == NULL)
		return node_create(data);

	while (list->next != NULL) {
		list = list->next;
	}

	MCHECK(list->next = malloc(sizeof(struct node_t)));
	list->next->next = NULL;
	list->next->data = data;
	return list->next;
}

struct node_t *node_last(struct node_t *list) {
	if (list == NULL)
		return NULL;

	while (list->next != NULL) {
		list = list->next;
	}
	return list;
}

struct node_t *node_unshift(struct node_t *list, void *data) {
	struct node_t *tmp = node_create(data);
	tmp->next		   = list;
	return tmp;
}

void *node_shift(struct node_t **list) {
	if (list == NULL || *list == NULL) {
		return NULL;
	}
	struct node_t *tmp = *list;
	void *data		   = tmp->data;
	*list			   = (*list)->next;
	free(tmp);
	return data;
}

struct node_t *node_append_int(struct node_t *list, int value) {
	if (list == NULL)
		return NULL;
	int *data;
	MCHECK(data = malloc(sizeof(int)));
	*data = value;
	return node_append(list, data);
}

unsigned int node_length(struct node_t *list) {
	unsigned int len = 0;
	while (list != NULL) {
		len++;
		list = list->next;
	}
	return len;
}

void node_destroy(struct node_t *node, int free_mem) {
	struct node_t *tmp;
	while (node != NULL) {
		tmp = node->next;
		if (free_mem)
			free(node->data);
		free(node);
		node = tmp;
	}
}

void *node_remove_last(struct node_t **list) {
	if((*list) == NULL)
		return NULL;

	void *data;
	if ((*list)->next == NULL) {
		data = (*list)->data;
		*list = NULL;
		node_destroy(*list, 0);
		return data;
	}

	struct node_t *second_last = *list;
	while(second_last->next->next != NULL) {
		second_last = second_last->next;
	}
	data = second_last->next->data;
	node_destroy(second_last->next, 0);
	second_last->next = NULL;

	return data;
}
