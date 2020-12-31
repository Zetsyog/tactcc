#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define can_log(level)          \
	{                           \
		if (_log_level < level) \
			return;             \
	}

static int _log_level = LOG_LEVEL_ALL;
int syntax_error = 0;

void set_log_level(int level) {
	_log_level = level;
}

void log_format(FILE *file, const char *tag, const char *message,
				va_list args) {
	time_t now;
	time(&now);
	char buffer[26];
	struct tm *tm_info;

	tm_info = localtime(&now);

	strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

	fprintf(file, "%s [%s]: ", buffer, tag);
	vfprintf(file, message, args);
	fprintf(file, "\n");
}

void log_debug(const char *message, ...) {
	can_log(LOG_LEVEL_DEBUG);
	//printf("\033[0;36m");
	va_list args;
	va_start(args, message);
	log_format(stdout, "DEBUG", message, args);
	va_end(args);
	//printf("\033[0m");
}

void log_info(const char *message, ...) {
	can_log(LOG_LEVEL_INFO);
	// printf("\033[1;37m");
	va_list args;
	va_start(args, message);
	log_format(stdout, "INFO", message, args);
	va_end(args);
	// printf("\033[0m");
}

void log_error(const char *message, ...) {
	can_log(LOG_LEVEL_ERROR);
	va_list args;
	va_start(args, message);
	// fprintf(stderr, "\033[1;31m");
	log_format(stderr, "ERROR", message, args);
	// fprintf(stderr, "\033[0m");
	va_end(args);
	exit(1);
}

void log_syntax_error(const char *message, ...) {
	syntax_error = 1;

	can_log(LOG_LEVEL_ERROR);
	fprintf(stderr, "\n");
	print_scanner_current_line();
	fprintf(stderr, "[ERROR] ");
	va_list args;
	va_start(args, message);
	// fprintf(stderr, "\033[1;31m");
	vfprintf(stderr, message, args);
	// fprintf(stderr, "\033[0m");
	va_end(args);
	fprintf(stderr, "\n");
}

void log_init() {
	const char *log_level_str = getenv("LOG_LEVEL");
	if (log_level_str != NULL) {
		set_log_level(atoi(log_level_str));
	}
}
