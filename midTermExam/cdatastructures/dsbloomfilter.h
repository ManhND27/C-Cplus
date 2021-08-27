/**
 * @file dsbloomfilter.h
 *
 * @brief Bloom filter
 *
 * A bloom filter is a space efficient data structure that can be
 * used to test whether a given element is part of a set.  Lookups
 * will occasionally generate false positives, but never false
 * negatives.
 *
 * To create a bloom filter, use @ref bloomfilter_new.  To destroy a
 * bloom filter, use @ref bloomfilter_free.
 *
 * To insert a value into a bloom filter, use @ref bloomfilter_insert.
 *
 * To query whether a value is part of the set, use
 * @ref bloomfilter_query.
 */

#ifndef DSBLOOMFILTER_H
#define DSBLOOMFILTER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A bloom filter structure.
 */

typedef struct _BloomFilter BloomFilter;

/**
 * A value stored in a @ref BloomFilter.
 */

typedef void *BloomFilterValue;

/**
 * Hash function used to generate hash values for values inserted into a
 * bloom filter.
 *
 * @param data   The value to generate a hash value for.
 * @return       The hash value.
 */

typedef unsigned int (*BloomFilterHashFunc)(BloomFilterValue data);

/**
 * Create a new bloom filter.
 *
 * @param tableSize        The size of the bloom filter.  The greater
 *                         the table size, the more elements can be
 *                         stored, and the lesser the chance of false
 *                         positives.
 * @param hashFunc         Hash function to use on values stored in the
 *                         filter.
 * @param numFunctions     Number of hash functions to apply to each
 *                         element on insertion.  This running time for
 *                         insertion and queries is proportional to this
 *                         value.  The more functions applied, the lesser
 *                         the chance of false positives.  The maximum
 *                         number of functions is 64.
 * @return                 A new hash table structure, or NULL if it
 *                         was not possible to allocate the new bloom
 *                         filter.
 */

BloomFilter *bloomfilter_new(unsigned int tableSize,
                             BloomFilterHashFunc hashFunc,
                             unsigned int numFunctions);

/**
 * Destroy a bloom filter.
 *
 * @param bloomfilter      The bloom filter to destroy.
 */

void bloomfilter_free(BloomFilter *bloomfilter);

/**
 * Insert a value into a bloom filter.
 *
 * @param bloomfilter          The bloom filter.
 * @param value                The value to insert.
 */

void bloomfilter_insert(BloomFilter *bloomfilter, BloomFilterValue value);

/**
 * Query a bloom filter for a particular value.
 *
 * @param bloomfilter          The bloom filter.
 * @param value                The value to look up.
 * @return                     Zero if the value was definitely not
 *                             inserted into the filter.  Non-zero
 *                             indicates that it either may or may not
 *                             have been inserted.
 */

int bloomfilter_query(BloomFilter *bloomfilter, BloomFilterValue value);

/**
 * Read the contents of a bloom filter into an array.
 *
 * @param bloomfilter          The bloom filter.
 * @param array                Pointer to the array to read into.  This
 *                             should be (table_size + 7) / 8 bytes in
 *                             length.
 */

void bloomfilter_read(BloomFilter *bloomfilter, unsigned char *array);

/**
 * Load the contents of a bloom filter from an array.
 * The data loaded should be the output read from @ref bloom_filter_read,
 * from a bloom filter created using the same arguments used to create
 * the original filter.
 *
 * @param bloomfilter          The bloom filter.
 * @param array                Pointer to the array to load from.  This
 *                             should be (table_size + 7) / 8 bytes in
 *                             length.
 */

void bloomfilter_load(BloomFilter *bloomfilter, unsigned char *array);

/**
 * Find the union of two bloom filters.  Values are present in the
 * resulting filter if they are present in either of the original
 * filters.
 *
 * Both of the original filters must have been created using the
 * same parameters to @ref bloom_filter_new.
 *
 * @param filter1              The first filter.
 * @param filter2              The second filter.
 * @return                     A new filter which is an intersection of the
 *                             two filters, or NULL if it was not possible
 *                             to allocate memory for the new filter, or
 *                             if the two filters specified were created
 *                             with different parameters.
 */

BloomFilter *bloomfilter_union(BloomFilter *filter1, BloomFilter *filter2);

/**
 * Find the intersection of two bloom filters.  Values are only ever
 * present in the resulting filter if they are present in both of the
 * original filters.
 *
 * Both of the original filters must have been created using the
 * same parameters to @ref bloom_filter_new.
 *
 * @param filter1              The first filter.
 * @param filter2              The second filter.
 * @return                     A new filter which is an intersection of the
 *                             two filters, or NULL if it was not possible
 *                             to allocate memory for the new filter, or
 *                             if the two filters specified were created
 *                             with different parameters.
 */

BloomFilter *bloomfilter_intersection(BloomFilter *filter1, BloomFilter *filter2);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSBLOOMFILTER_H */

