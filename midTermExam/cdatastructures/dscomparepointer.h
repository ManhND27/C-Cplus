/**
 * @file dscomparepointer.h
 *
 * Comparison functions for generic (void) pointers.
 *
 * To find the difference between two pointers, use @ref pointerCompare.
 *
 * To find if two pointers are equal, use @ref pointerEqual.
 */

#ifndef DSCOMPAREPOINTER_H
#define DSCOMPAREPOINTER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compare two pointers to determine if they are equal.
 *
 * @param location1       The first pointer.
 * @param location2       The second pointer.
 * @return                Non-zero if the pointers are equal, zero if they
 *                        are not equal.
 */

int pointerEqual(void *location1, void *location2);

/**
 * Compare two pointers.
 *
 * @param location1       The first pointer.
 * @param location2       The second pointer.
 * @return                A negative value if the first pointer is in a lower
 *                        memory address than the second, a positive value if
 *                        the first pointer is in a higher memory address than
 *                        the second, zero if they point to the same location.
 */

int pointerCompare(void *location1, void *location2);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSCOMPAREPOINTER_H */

