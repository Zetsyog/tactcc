/**
 * @file
 */
#ifndef UTIL_H
#define UTIL_H

#include "generation/defs.h"
#include "logger.h"
#include "util/node.h"

#define MCHECK(op)                       \
	do {                                 \
		if ((op) == NULL) {              \
			log_error("%s failed", #op); \
			exit(1);                     \
		}                                \
	} while (0)

#endif

extern struct node_t *garbage;

int get_next_char(char *b);
void print_scanner_current_line();
void destroy_scanner();
void clean_exit(int exitcode);
