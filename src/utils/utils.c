#include "utils.h"

#include <stdlib.h>
#include <string.h>

static char warn_string[1000] = {0};

void rast_add_warn_msg(const char *str) {
	if (!str) return;
	size_t len = strlen(warn_string);
	size_t remaining = sizeof(warn_string) - len - 1; // leave room for '\0'

	if (remaining > 0) {
		strncat(warn_string, str, remaining);
	}
}

char *rast_get_msg() { return warn_string; }
