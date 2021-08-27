#include "dshashint.h"

/* Hash function for a pointer to an integer */

unsigned int intHash(void *vlocation)
{
    int *location = (int *) vlocation;

	return (unsigned int) *location;
}

