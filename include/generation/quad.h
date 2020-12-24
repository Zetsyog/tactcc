#ifndef GEN_QUAD_H
#define GEN_QUAD_H

enum operation_t {
	ASSIGNMENT		  = 0,
	ADD				  = 1,
	MINUS			  = 2,
	MULTIPLIES		  = 3,
	DIVIDES			  = 4,
	POWER			  = 5,
	LOWER			  = 6,
	LOWER_OR_EQUAL	  = 7,
	SUPERIOR		  = 8,
	SUPERIOR_OR_EQUAL = 9,
	EQUALS			  = 10,
	DIFFERENT		  = 11,
	GOTO			  = 12

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

int nextquad;
struct quad_t tabQuad[10000];

#endif