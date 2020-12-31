#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUF_SIZE 1024

extern FILE *yyin;

static char buffer[LINE_BUF_SIZE] = {0};
static char *cur_line			  = NULL;
static size_t cur_line_len;
static size_t cur_line_idx;
static size_t line_nb = 0;

static int get_next_line(void) {
	line_nb++;
	if (fgets(buffer, LINE_BUF_SIZE, yyin) == NULL) {
		cur_line_idx = 0;
		cur_line[0]	 = 0;
		return EOF;
	}
	cur_line_len = strlen(buffer);
	MCHECK(cur_line = realloc(cur_line, cur_line_len + 1));
	strncpy(cur_line, buffer, cur_line_len);
	cur_line[cur_line_len] = 0;
	cur_line_idx		   = 0;
	return 0;
}

void print_scanner_current_line() {
	for (size_t i = 0; i < cur_line_len; i++) {
		if (cur_line[i] == '\t')
			cur_line[i] = ' ';
	}
	fprintf(stderr,
			"|....+....:....+....:....+....:....+....:....+\n");
	fprintf(stderr, "%-5lu| %s", line_nb, cur_line);
	fprintf(stderr, ".... ! ");
	size_t i;
	for (i = 0; i < cur_line_idx; i++) {
		fprintf(stderr, ".");
	}
	fprintf(stderr, "^");
	i++;
	for (; i < cur_line_len; i++) {
		fprintf(stderr, ".");
	}
	fprintf(stderr, "\n");
}

int get_next_char(char *b) {
	while (cur_line_idx >= cur_line_len) {
		if (get_next_line()) {
			return EOF;
		}
	}
	*b = cur_line[cur_line_idx];
	cur_line_idx++;

	return *b == 0 ? 0 : 1;
}

void destroy_scanner() {
	free(cur_line);
}
