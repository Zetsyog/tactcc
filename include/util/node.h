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

/**
 * Create a new linked list
 * @param data The data of the node
 * @return The created node
 */
struct node_t *node_create(void *data);
struct node_t *node_create_int(int value);
/**
 * Add a new node with the given data at the end of the list
 * @param list The list
 * @param data The new node data
 * @return The new node
 */
struct node_t *node_append(struct node_t *list, void *data);
struct node_t *node_append_int(struct node_t *list, int data);

/**
 * Add a new node with the given data at the beginning of the list
 * @param list The list
 * @param data The new node data
 * @return The new beginning of the list
 */
struct node_t *node_unshift(struct node_t *list, void *data);

/**
 * Remove the first element of the list and return its value \n
 * Free the node memory but not the value
 * @param list Pointer to the list (Be careful: pointer of pointer)
 * @return The data of the removed node
 */
void *node_shift(struct node_t **list);

/**
 * @param list
 * @return The length of the given list
 */
unsigned int node_length(struct node_t *list);

/**
 * @brief Removed the last element of the list
 * 
 * @param list 
 * @return void* the removed element
 */
void *node_remove_last(struct node_t **list);

/**
 * Destroy and free memory of list and all its children \n
 * If free_mem is != 0, it will call free on all the node_t::data elements
 * @param list
 * @param free_mem
 */
void node_destroy(struct node_t *list, int free_mem);

/**
 * Add a new node with the given data at the beginning of the list
 * @param list The list
 * @param data The new node data
 * @return The new beginning of the list
 */
struct node_t *node_unshift(struct node_t *list, void *data);

/**
 * Remove the first element of the list and return its value \n
 * Free the node memory but not the value
 * @param list Pointer to the list (Be careful: pointer of pointer)
 * @return The data of the removed node
 */
void *node_shift(struct node_t **list);

/**
 * @param list
 * @return The length of the given list
 */
unsigned int node_length(struct node_t *list);

#endif
