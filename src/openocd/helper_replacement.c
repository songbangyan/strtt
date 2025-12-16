#include "helper_types.h"
#include "helper_replacements.h"

/* strndup is not available in MSVC or MinGW */
#ifdef _WIN32
char *strndup(const char *s, size_t n)
{
	size_t len = strnlen(s, n);
	char *new = malloc(len + 1);

	if (!new)
		return NULL;

	new[len] = '\0';
	return (char *) memcpy(new, s, len);
}
#endif
