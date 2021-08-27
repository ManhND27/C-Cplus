#include "dscomparepointer.h"

/* Comparison functions for a generic void pointer */

int pointerEqual(void *location1, void *location2)
{
	return location1 == location2;
}

int pointerCompare(void *location1, void *location2)
{
	if (location1 < location2) {
		return -1;
	} else if (location1 > location2) {
		return 1;
	} else {
		return 0;
	}
}


