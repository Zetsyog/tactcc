#ifndef GEN_QUAD_H
#define GEN_QUAD_H

enum operation_t {
	OP_ASSIGNMENT		 = 0,
	OP_ADD				 = 1,
	OP_MINUS			 = 2,
	OP_MULTIPLIES		 = 3,
	OP_DIVIDES			 = 4,
	OP_POWER			 = 5,
	OP_LOWER			 = 6,
	OP_LOWER_OR_EQUAL	 = 7,
	OP_SUPERIOR			 = 8,
	OP_SUPERIOR_OR_EQUAL = 9,
	OP_EQUALS			 = 10,
	OP_DIFFERENT		 = 11,
	OP_GOTO				 = 12,
	OP_NEGATE			 = 13,
	OP_AND				 = 14,
	OP_OR				 = 15,
	OP_XOR				 = 16,
	OP_NOT				 = 17

};

struct quad_t {
	enum operation_t op;
	void *arg1;
	void *arg2;
	void *res;
};

struct list_t {
	int position;
	struct list_t *next;
};

unsigned int nextquad;
struct quad_t tabQuad[10000];

#endif