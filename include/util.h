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
