#include "generation/defs.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbol_t *newtemp(enum sym_type_t sym_type,
						 enum atomic_type_t atomic_type, ...) {
	static unsigned int nextTmp = 0;
	char name[SYM_NAME_MAX_LEN] = {0};
	va_list args;
	va_start(args, atomic_type);
	int n;
	do {
		if(sym_type == SYM_CST) {
			n = snprintf(name, SYM_NAME_MAX_LEN - 1, "%s%u", CONST_PREFIX, nextTmp);
		} else {
			n = snprintf(name, SYM_NAME_MAX_LEN - 1, "%s%u", TMP_VAR_PREFIX, nextTmp);
		}
		name[n] = 0;
		nextTmp++;
	} while (st_get(name) != NULL);

	struct symbol_t *sym = sym_create(name, sym_type, atomic_type);
	if (sym_type == SYM_CST) {
		if (atomic_type == A_INT || atomic_type == A_BOOL) {
			sym->int_val = va_arg(args, int);
		} else if (atomic_type == A_STR) {
			char *str  = va_arg(args, char *);
			size_t len = strlen(str);
			MCHECK(sym->str_val = calloc(1, len + 1));
			strncpy(sym->str_val, str, len);
		} else {
			log_syntax_error("syntax error: unsupported constant type %s",
					  atomic_type_str[atomic_type]);
		}
	} else if (sym_type == SYM_VAR) {
		
	} else {
		log_syntax_error("syntax error: unsupported tmp var type");
	}

	st_put(sym);

	va_end(args);

	return sym;
}
