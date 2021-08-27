/**
 * @file dshashstring.h
 *
 * Hash functions for text strings.  For more information
 * see @ref stringHash or @ref stringNocaseHash.
 */

#ifndef DSHASHSTRING_H
#define DSHASHSTRING_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generate a hash key from a string.
 *
 * @param string           The string.
 * @return                 A hash key for the string.
 */

unsigned int stringHash(void *string);

/**
 * Generate a hash key from a string, ignoring the case of letters.
 *
 * @param string           The string.
 * @return                 A hash key for the string.
 */

unsigned int stringNocaseHash(void *string);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSHASHSTRING_H */

