#ifndef OPTIONS_H
#define OPTIONS_H

void parse_opt(int argc, char **argv);

extern struct options_t {
	char *output_path;
	unsigned int print_tos : 1;
} options;

#endif