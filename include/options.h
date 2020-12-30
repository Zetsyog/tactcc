/**
 * @file
 */
#ifndef OPTIONS_H
#define OPTIONS_H

void parse_opt(int argc, char **argv);

/**
 * @struct
 * Global structure containing program options
 */
extern struct options_t {
	char *output_path;
	unsigned int print_tos : 1;
} options;

#endif
