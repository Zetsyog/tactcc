#ifndef UTIL_H
#define UTIL_H

#include "logger.h"
#include "util/node.h"
#include "generation/defs.h"

#define MCHECK(op)                       \
	do {                                 \
		if ((op) == NULL) {              \
			log_error("%s failed", #op); \
			exit(1);                     \
		}                                \
	} while (0)

struct symbol_t *to_sym(void *arg);

#endif