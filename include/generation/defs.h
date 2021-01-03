/**
 * @file
 */
#ifndef GEN_DEF_H
#define GEN_DEF_H

#include "generation/quad.h"
#include "generation/symtable.h"

typedef unsigned int uint;

/**
 * @brief Value of a expr token in grammar
 * @see grammar.y
 *
 */
struct expr_val_t {
	/**
	 * @brief Atomic type of expression
	 *
	 */
	enum atomic_type_t a_type;
	/**
	 * @brief Symbol variable of expression
	 * If expression is of int type, then this variable can be used
	 * Do not use else
	 */
	struct symbol_t *ptr;
	/**
	 * @brief Chained list used for boolean computation
	 * Contains a list of uncomplete GOTO quads that will be filled by the quad
	 * to go when the expr is true
	 */
	struct list_t * true;
	/**
	 * @brief Chained list used for boolean computation
	 * Contains a list of uncomplete GOTO quads that will be filled by the quad
	 * to go when the expr is false
	 */
	struct list_t * false;
};

/**
 * @brief Generate a new quad
 * The function take minimum 2 arguments, up to 4 arguments depending on the
 * operation \n
 * The order is always (op, args, res|label) \n
 * args are always * struct symbol_t * \n
 * res is a struct symbol_t * \n
 * label is a struct quad_t * \n
 * \n
 * Ex : gencode(OP_LOWER, arg1, arg2, dest) \n
 *      gencode(OP_GOTO, quad) \n
 * 	    gencode(OP_EXIT, NULL)
 *
 * @see struct quad_t
 *
 * @param op Quad operation
 * @param ... The params of the quad
 * @return int
 */
int gencode(uint op, ...);

/**
 * Action functions are function that performs actions in the .y file
 */

/**
 * @brief Perform function call action
 *
 * @param func
 * @param expr_list
 */
void action_call(struct symbol_t *func, struct node_t *expr_list);

/**
 * @brief evaluate an expression and save it in a tmp symbol
 *
 * @param expr
 * @return struct symbol_t*
 */
struct symbol_t *action_eval_par(struct expr_val_t expr);

/**
 * @brief Perform assignment action
 *
 * @param sym
 * @param expr
 * @return struct list_t*
 */
struct list_t *action_assign(struct symbol_t *sym, struct expr_val_t expr);

/**
 * @brief Perform binary operation between arg1 and arg2
 *
 * @param arg1
 * @param op
 * @param arg2
 * @param marker index of the first quad of arg2
 * @return struct expr_val_t
 */
struct expr_val_t action_opb(struct expr_val_t arg1, enum operation_t op,
							 struct expr_val_t arg2, unsigned int marker);

/**
 * @brief Create a new linked list of quad index
 *
 * @param pos
 * @return struct list_t*
 */
struct list_t *crelist(int pos);

/**
 * @brief Concat two lists
 *
 * @param list1
 * @param list2
 * @return struct list_t*
 */
struct list_t *concat(struct list_t *list1, struct list_t *list2);

/**
 * @brief Create a new temporary symbol
 *
 * @param ...
 * @return struct symbol_t*
 */
struct symbol_t *newtemp(enum sym_type_t, enum atomic_type_t, ...);

/**
 * @brief Complete a list of quads with the given quad index
 *
 * @param list
 * @param pos
 */
void complete(struct list_t *list, unsigned int pos);

/**
 * @brief Destroyed a linked list
 *
 * @param list
 */
void destroy_list(struct list_t *list);

#endif
