/**
 * @file dshashpointer.h
 *
 * Hash function for a generic (void) pointer.  See @ref pointerHash.
 */

#ifndef DSHASHPOINTER_H
#define DSHASHPOINTER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generate a hash key for a pointer.  The value pointed at by the pointer
 * is not used, only the pointer itself.
 *
 * @param location        The pointer
 * @return                A hash key for the pointer.
 */

unsigned int pointerHash(void *location);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSHASHPOINTER_H */

