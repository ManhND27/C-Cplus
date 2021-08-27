/**
 * @file dscompareint.h
 *
 * Comparison functions for pointers to integers.
 *
 * To find the difference between two values pointed at, use
 * @ref intCompare.
 *
 * To find if two values pointed at are equal, use @ref intEqual.
 */

#ifndef DSCOMPAREINT_H
#define DSCOMPAREINT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compare the integer values pointed at by two pointers to determine
 * if they are equal.
 *
 * @param location1       Pointer to the first value to compare.
 * @param location2       Pointer to the second value to compare.
 * @return                Non-zero if the two values are equal, zero if the
 *                        two values are not equal.
 */

int intEqual(void *location1, void *location2);

/**
 * Compare the integer values pointed at by two pointers.
 *
 * @param location1        Pointer to the first value to compare.
 * @param location2        Pointer to the second value to compare.
 * @return                 A negative value if the first value is less than
 *                         the second value, a positive value if the first
 *                         value is greater than the second value, zero if
 *                         they are equal.
 */

int intCompare(void *location1, void *location2);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSCOMPAREINT_H */

