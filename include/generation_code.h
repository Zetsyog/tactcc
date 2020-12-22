#include "Intermediate_code.h"
#include "grammar.h"

int gencode(Operation op, ...);
int* crelist(int addr);
int* concat(int* list1, int* list2);
void newtemp();
void complete(int* list, int addr);
