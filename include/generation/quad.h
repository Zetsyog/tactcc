#ifndef GEN_QUAD_H
#define GEN_QUAD_H

enum operation_t {
	OP_ASSIGNMENT		 = 0,  /* res := x */
	OP_NEGATE			 = 1,  /* res := -x */
	OP_ADD				 = 2,  /* res := x + y */
	OP_MINUS			 = 3,  /* res := x - y */
	OP_MULTIPLIES		 = 4,  /* res := x * y */
	OP_DIVIDES			 = 5,  /* res := x / y */
	OP_POWER			 = 6,  /* res := x ^ y */
	OP_LOWER			 = 7,  /* res := x < y or if(x < y) goto label */
	OP_LOWER_OR_EQUAL	 = 8,  /* idem with <= */
	OP_SUPERIOR			 = 9,  /* idem with > */
	OP_SUPERIOR_OR_EQUAL = 10, /* idem with >= */
	OP_EQUALS			 = 11, /* idem with == */
	OP_DIFFERENT		 = 12, /* idem with <> */
	OP_AND				 = 13, /* idem with and */
	OP_OR				 = 14, /* idem with or */
	OP_XOR				 = 15, /* idem with xor */
	OP_NOT				 = 16, /* idem with not */
	OP_GOTO				 = 17, /* goto label */
	OP_WRITE			 = 18, /* write x */
	OP_READ				 = 19, /* read x */
	OP_IF				 = 20,
	OP_WHILE			 = 21
};

struct quad_t {
	unsigned int id;
	enum operation_t op;
	unsigned int print_label : 1;
	struct symbol_t *arg1;
	struct symbol_t *arg2;
	union {
		struct symbol_t *res;
		struct quad_t *label;
	};
};

struct list_t {
	int position;
	struct list_t *next;
};

const char *get_quad_name(struct quad_t *quad);

extern unsigned int nextquad;
extern struct quad_t tabQuad[10000];

#endif
