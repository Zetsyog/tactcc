#include "options.h"
#include "util.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "Usage: %s [-o, -output <file>] [-v, -version] [-?, -help] [-tos]"

struct options_t options = {0};

void print_version() {
	printf("=== Projet de compilation 2020/2021 ===\n");
	printf("Codé par :\n");
	printf("\t* Téo DUCAROUGE\n");
	printf("\t* Thomas LEPINE\n");
	printf("\t* Alexandre MELKONIAN\n");
	printf("\t* Clément ROSSETTI\n");

	exit(EXIT_SUCCESS);
}

void print_usage(char *exec_name) {
	printf(USAGE, exec_name);
	printf("\n");
}

void parse_opt(int argc, char **argv) {
	int c;

	while (1) {
		int option_index					= 0;
		static struct option long_options[] = {
			[0] = {"version", no_argument, 0, 0},
			[1] = {"tos", no_argument, 0, 0},
			[2] = {"output", required_argument, 0, 0},
			[3] = {"help", no_argument, 0, 0},
			{0, 0, 0, 0}};

		c = getopt_long_only(argc, argv, "?vo:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			if (option_index == 0) {
				print_version();
				exit(EXIT_SUCCESS);
			} else if (option_index == 1) {
				options.print_tos = 1;
			} else if (option_index == 2) {
				if (!optarg) {
					log_error("arg is required for output option");
					exit(EXIT_FAILURE);
				}
				options.output_path = optarg;
			} else if (option_index == 3) {
				print_usage(argv[0]);
				exit(EXIT_SUCCESS);
			}
			printf("\n");
			break;
		case 'o':
			options.output_path = optarg;
			break;
		case 'v':
			print_version();
			exit(EXIT_SUCCESS);
			break;
		case '?':
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);
			break;
		default:
			log_error("Unknown option %c\n", c);
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if (optind < argc) {
		printf("Arguments ne constituant pas des options : ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}
}
