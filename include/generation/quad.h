/**
 * @file
 */

#ifndef GEN_QUAD_H
#define GEN_QUAD_H

#define IS_BOOL_OP(op) op >= OP_LOWER &&op <= OP_NOT

enum operation_t {
	OP_ASSIGNMENT = 0, /** res := x */
	// Expr op
	OP_NEGATE	  = 1, /** res := -x */
	OP_ADD		  = 2, /** res := x + y */
	OP_MINUS	  = 3, /** res := x - y */
	OP_MULTIPLIES = 4, /** res := x * y */
	OP_DIVIDES	  = 5, /** res := x / y */
	OP_POWER	  = 6, /** res := x ^ y */
	// Bool op
	OP_LOWER			 = 7,  /** res := x < y or if(x < y) goto label */
	OP_LOWER_OR_EQUAL	 = 8,  /** idem with <= */
	OP_SUPERIOR			 = 9,  /** idem with > */
	OP_SUPERIOR_OR_EQUAL = 10, /** idem with >= */
	OP_EQUALS			 = 11, /** idem with == */
	OP_DIFFERENT		 = 12, /** idem with <> */
	OP_AND				 = 13, /** idem with and */
	OP_OR				 = 14, /** idem with or */
	OP_XOR				 = 15, /** idem with xor */
	OP_NOT				 = 16, /** idem with not */
	// Other
	OP_GOTO		= 17, /** goto label */
	OP_WRITE	= 18, /** write x */
	OP_READ		= 19, /** read x */
	OP_CALL		= 20, /** call function */
	OP_RETURN	= 21, /** return from function */
	OP_PUSH_ARG = 22,
	OP_POP_ARG	= 23,
	OP_PUSH_RET = 24,
	OP_POP_RET  = 25
};

struct quad_t {
	unsigned int id;
	enum operation_t op;
	unsigned int print_label : 1;
	unsigned int is_main : 1;
	struct symbol_t *arg1;
	struct symbol_t *arg2;
	union {
		struct symbol_t *res;
		struct quad_t *label;
	};
};

/**
 * @struct Linked list of quad positions
 * Used for goto generation
 * @see crelist()
 * @see complete()
 * @see concat()
 */
struct list_t {
	/**
	 * A quad position
	 */
	int position;
	/**
	 * The next element of the list
	 */
	struct list_t *next;
};

extern unsigned int nextquad;
extern struct quad_t tabQuad[10000];

#endif
