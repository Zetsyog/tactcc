#ifndef GEN_DEF_H
#define GEN_DEF_H

typedef enum {
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

} operation;

struct quad {
	operation op;
	void *arg1;
	void *arg2;
	void *res;
};

typedef unsigned int uint;

int nextquad;
struct quad *tabQuad[10000];

int gencode(operation op, ...);
int *crelist(int addr);
int *concat(int *list1, int *list2);
void newtemp();
void complete(int *list, int addr);

#endif