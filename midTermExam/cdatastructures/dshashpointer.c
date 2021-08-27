#include <limits.h>

#include "dshashpointer.h"

/* Hash function for a generic pointer */

unsigned int pointerHash(void *location)
{
	return (unsigned int) (unsigned long) location;
}

