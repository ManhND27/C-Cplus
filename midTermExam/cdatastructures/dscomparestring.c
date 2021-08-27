#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dscomparestring.h"

/* Comparison functions for strings */

int stringEqual(void *string1, void *string2)
{
	return strcmp((char *) string1, (char *) string2) == 0;
}

int stringCompare(void *string1, void *string2)
{
    int result = strcmp((char *) string1, (char *) string2);

	if (result < 0) {
		return -1;
	} else if (result > 0) {
		return 1;
	} else {
		return 0;
	}
}

/* Comparison functions for strings, which ignore the case of letters. */

int stringNocaseEqual(void *string1, void *string2)
{
	return stringNocaseCompare((char *) string1, (char *) string2) == 0;
}

/* On many systems, strcasecmp or stricmp will give the same functionality
 * as this function.  However, it is non-standard and cannot be relied
 * on to be present. */

int stringNocaseCompare(void *string1, void *string2)
{
	/* Iterate over each character in the strings */

    char *p1 = (char *) string1;
    char *p2 = (char *) string2;
    int c1;
    int c2;

	for (;;) {

		c1 = tolower(*p1);
		c2 = tolower(*p2);

		if (c1 != c2) {

			/* Strings are different */

			if (c1 < c2) {
				return -1;
			} else {
				return 1;
			}
		}

		/* End of string */

		if (c1 == '\0')
			break;

		/* Advance to the next character */

		++p1;
		++p2;
	}

	/* Reached the end of string and no difference found */

	return 0;
}
