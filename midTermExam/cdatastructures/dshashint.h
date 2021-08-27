/**
 * @file dshashint.h
 *
 * Hash function for a pointer to an integer.  See @ref intHash.
 */

#ifndef DSHASHINT_H
#define DSHASHINT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generate a hash key for a pointer to an integer.  The value pointed
 * at is used to generate the key.
 *
 * @param location        The pointer.
 * @return                A hash key for the value at the location.
 */

unsigned int intHash(void *location);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSHASHINT_H */

