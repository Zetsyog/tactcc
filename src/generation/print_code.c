#include "generation/print_code.h"
#include "generation/defs.h"
#include "grammar.h"
#include <stdio.h>

void print_intermediate_code() {
	for (int i = 0; i < nextquad; i++) {
		printf("%i\t", i + 1);
		struct quad *quad = tabQuad[i];

		switch (quad->op) {
		case ASSIGNMENT:
			printf("%s := %s\n", (char *)quad->res, (char *)quad->arg1);
			break;
		case ADD:
			printf("%s := %s + %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case MINUS:
			printf("%s := %s - %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case MULTIPLIES:
			printf("%s := %s * %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case DIVIDES:
			printf("%s := %s / %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case POWER:
			printf("%s := %s ^ %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case LOWER:
			printf("%s := %s < %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case LOWER_OR_EQUAL:
			printf("%s := %s <= %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case SUPERIOR:
			printf("%s := %s > %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case SUPERIOR_OR_EQUAL:
			printf("%s := %s >= %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case EQUALS:
			printf("%s := %s = %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
		case DIFFERENT:
			printf("%s := %s <> %s\n", (char *)quad->res, (char *)quad->arg1,
				   (char *)quad->arg2);
			break;
        }
	}
}
