/**
 * @file dshashtable.h
 *
 * @brief Hash table.
 *
 * A hash table stores a set of values which can be addressed by a
 * key.  Given the key, the corresponding value can be looked up
 * quickly.
 *
 * To create a hash table, use @ref hashtable_new.  To destroy a
 * hash table, use @ref hashtable_free.
 *
 * To insert a value into a hash table, use @ref hashtable_insert.
 *
 * To remove a value from a hash table, use @ref hashtable_remove.
 *
 * To look up a value by its key, use @ref hashtable_lookup.
 *
 * To iterate over all values in a hash table, use
 * @ref hashtable_iterate to initialise a @ref HashTableIterator
 * structure.  Each value can then be read in turn using
 * @ref hashtable_iteratorNext and @ref hashtable_iteratorHasMore.
 */

#ifndef DSHASHTABLE_H
#define DSHASHTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A hash table structure.
 */

typedef struct _HashTable HashTable;

/**
 * Structure used to iterate over a hash table.
 */

typedef struct _HashTableIterator HashTableIterator;

/**
 * Internal structure representing an entry in a hash table.
 */

typedef struct _HashTableEntry HashTableEntry;

/**
 * A key to look up a value in a @ref HashTable.
 */

typedef void *HashTableKey;

/**
 * A value stored in a @ref HashTable.
 */

typedef void *HashTableValue;

/**
 * Internal structure representing an entry in hash table
 * used as @ref HashTableIterator next result.
 */

typedef struct _HashTablePair{
	HashTableKey key;
	HashTableValue value;
} HashTablePair;

/**
 * Definition of a @ref HashTableIterator.
 */

struct _HashTableIterator {
    HashTable *hashTable;
    HashTableEntry *nextEntry;
    unsigned int nextChain;
};

/**
 * A null @ref HashTableValue.
 */

#define HASH_TABLE_NULL ((void *) 0)

/**
 * Hash function used to generate hash values for keys used in a hash
 * table.
 *
 * @param value  The value to generate a hash value for.
 * @return       The hash value.
 */

typedef unsigned int (*HashTableHashFunc)(HashTableKey value);

/**
 * Function used to compare two keys for equality.
 *
 * @return   Non-zero if the two keys are equal, zero if the keys are
 *           not equal.
 */

typedef int (*HashTableEqualFunc)(HashTableKey value1, HashTableKey value2);

/**
 * Type of function used to free keys when entries are removed from a
 * hash table.
 */

typedef void (*HashTableKeyFreeFunc)(HashTableKey value);

/**
 * Type of function used to free values when entries are removed from a
 * hash table.
 */

typedef void (*HashTableValueFreeFunc)(HashTableValue value);

/**
 * Create a new hash table.
 *
 * @param hashFunc             Function used to generate hash keys for the
 *                             keys used in the table.
 * @param equalFunc            Function used to test keys used in the table
 *                             for equality.
 * @return                     A new hash table structure, or NULL if it
 *                             was not possible to allocate the new hash
 *                             table.
 */

HashTable *hashtable_new(HashTableHashFunc hashFunc,
                         HashTableEqualFunc equalFunc);

/**
 * Destroy a hash table.
 *
 * @param hashTable            The hash table to destroy.
 */

void hashtable_free(HashTable *hashTable);

/**
 * Register functions used to free the key and value when an entry is
 * removed from a hash table.
 *
 * @param hashTable            The hash table.
 * @param keyFreeFunc          Function used to free keys.
 * @param valueFreeFunc        Function used to free values.
 */

void hashtable_registerFreeFunctions(HashTable *hashTable,
                                     HashTableKeyFreeFunc keyFreeFunc,
                                     HashTableValueFreeFunc valueFreeFunc);

/**
 * Insert a value into a hash table, overwriting any existing entry
 * using the same key.
 *
 * @param hashTable            The hash table.
 * @param key                  The key for the new value.
 * @param value                The value to insert.
 * @return                     Non-zero if the value was added successfully,
 *                             or zero if it was not possible to allocate
 *                             memory for the new entry.
 */

int hashtable_insert(HashTable *hashTable,
                     HashTableKey key,
                     HashTableValue value);

/**
 * Look up a value in a hash table by key.
 *
 * @param hashTable           The hash table.
 * @param key                 The key of the value to look up.
 * @return                    The value, or @ref HASH_TABLE_NULL if there
 *                            is no value with that key in the hash table.
 */

HashTableValue hashtable_lookup(HashTable *hashTable,
                                HashTableKey key);

/**
 * Remove a value from a hash table.
 *
 * @param hashTable           The hash table.
 * @param key                 The key of the value to remove.
 * @return                    Non-zero if a key was removed, or zero if the
 *                            specified key was not found in the hash table.
 */

int hashtable_remove(HashTable *hashTable, HashTableKey key);

/**
 * Retrieve the number of entries in a hash table.
 *
 * @param hashTable           The hash table.
 * @return                    The number of entries in the hash table.
 */

unsigned int hashtable_numEntries(HashTable *hashTable);

/**
 * Initialise a @ref HashTableIterator to iterate over a hash table.
 *
 * @param hashTable           The hash table.
 * @param iter                Pointer to an iterator structure to
 *                            initialise.
 */

void hashtable_iterate(HashTable *hashTable, HashTableIterator *iter);

/**
 * Determine if there are more keys in the hash table to iterate
 * over.
 *
 * @param iterator            The hash table iterator.
 * @return                    Zero if there are no more values to iterate
 *                            over, non-zero if there are more values to
 *                            iterate over.
 */

int hashtable_iteratorHasMore(HashTableIterator *iterator);

/**
 * Using a hash table iterator, retrieve the next @ref HashTablePair.
 *
 * Note: To avoid @ref HashTableEntry internal @ref HashTablePair
 *       from being tampered with, and potentially messing with
 *       internal table structure, the function returns a copy
 *       of @ref HashTablePair stored internally.
 *
 * @param iterator            The hash table iterator.
 * @return                    The next @ref HashTablePair from the hash
 *                            table, or @ref HASH_TABLE_NULL of Key and
 *                            Value if there are no more keys to iterate
 *                            over.
 */

HashTablePair hashtable_iteratorNext(HashTableIterator *iterator);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSHASHTABLE_H */

