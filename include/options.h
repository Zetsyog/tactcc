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
	char *output_path; /** The output file where to write assembly code */
	unsigned int
		print_tos : 1; /** If true, write the table of symbol in stdout */
} options;

#endif
