/**
 * @file
 */
#ifndef UTIL_H
#define UTIL_H

#include "generation/defs.h"
#include "logger.h"
#include "util/node.h"

/**
 * @brief Check macro for memory allocation
 * On error, it writes in stderr and then exit the program
 *
 */
#define MCHECK(op)                       \
	do {                                 \
		if ((op) == NULL) {              \
			log_error("%s failed", #op); \
			exit(1);                     \
		}                                \
	} while (0)

#endif

extern struct node_t *garbage;
/**
 * @brief Get the next char object from yyin
 * Read a char and put it in b
 * Used as custom input for lex reading
 * @see YY_INPUT
 * 
 * @param b dest where to put the read char
 * @return 0 if *b equals 0, else return 1
 */
int get_next_char(char *b);

/**
 * @brief Print in stderr the current input line
 * Display a pretty cursor that point the current char
 *  
 */
void print_scanner_current_line();

/**
 * @brief Free memory used for the custom scanner
 * 
 */
void destroy_scanner();

/**
 * @brief Free allocated memory and call exit()
 * 
 * @param exitcode 
 */
void clean_exit(int exitcode);
