/* Hash table implementation */

#include <stdlib.h>
#include <string.h>

#include "dshashtable.h"


struct _HashTableEntry {
	HashTablePair pair;
	HashTableEntry *next;
};

struct _HashTable {
	HashTableEntry **table;
    unsigned int tableSize;
    HashTableHashFunc hashFunc;
    HashTableEqualFunc equalFunc;
    HashTableKeyFreeFunc keyFreeFunc;
    HashTableValueFreeFunc valueFreeFunc;
	unsigned int entries;
    unsigned int primeIndex;
};

/* This is a set of good hash table prime numbers, from:
 *   http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
 * Each prime is roughly double the previous value, and as far as
 * possible from the nearest powers of two. */

static const unsigned int HASH_TABLE_PRIMES[] = {
	193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
	196613, 393241, 786433, 1572869, 3145739, 6291469,
	12582917, 25165843, 50331653, 100663319, 201326611,
	402653189, 805306457, 1610612741,
};

static const unsigned int HASH_TABLE_NUM_PRIMES
    = sizeof(HASH_TABLE_PRIMES) / sizeof(int);

/* Internal function used to allocate the table on hash table creation
 * and when enlarging the table */

static int hashtable_allocateTable(HashTable *hashTable)
{
	/* Determine the table size based on the current prime index.
	 * An attempt is made here to ensure sensible behavior if the
	 * maximum prime is exceeded, but in practice other things are
	 * likely to break long before that happens. */

    unsigned int newTableSize;

    if (hashTable->primeIndex < HASH_TABLE_NUM_PRIMES) {
        newTableSize = HASH_TABLE_PRIMES[hashTable->primeIndex];
	} else {
        newTableSize = hashTable->entries * 10;
	}

    hashTable->tableSize = newTableSize;

	/* Allocate the table and initialise to NULL for all entries */

    hashTable->table = calloc(hashTable->tableSize,
                              sizeof(HashTableEntry *));

    return hashTable->table != NULL;
}

/* Free an entry, calling the free functions if there are any registered */

static void hashtable_freeEntry(HashTable *hashTable, HashTableEntry *entry)
{
    HashTablePair *pair = &(entry->pair);

	/* If there is a function registered for freeing keys, use it to free
	 * the key */

    if (hashTable->keyFreeFunc != NULL) {
        hashTable->keyFreeFunc(pair->key);
	}

	/* Likewise with the value */

    if (hashTable->valueFreeFunc != NULL) {
        hashTable->valueFreeFunc(pair->value);
	}

	/* Free the data structure */

	free(entry);
}

HashTable *hashtable_new(HashTableHashFunc hashFunc,
                         HashTableEqualFunc equalFunc)
{
	/* Allocate a new hash table structure */

    HashTable *hashTable = (HashTable *) malloc(sizeof(HashTable));

    if (hashTable == NULL) {
		return NULL;
	}

    hashTable->hashFunc = hashFunc;
    hashTable->equalFunc = equalFunc;
    hashTable->keyFreeFunc = NULL;
    hashTable->valueFreeFunc = NULL;
    hashTable->entries = 0;
    hashTable->primeIndex = 0;

	/* Allocate the table */

    if (!hashtable_allocateTable(hashTable)) {
        free(hashTable);

		return NULL;
	}

    return hashTable;
}

void hashtable_free(HashTable *hashTable)
{
	HashTableEntry *rover;
	HashTableEntry *next;
	unsigned int i;

	/* Free all entries in all chains */

    for (i=0; i<hashTable->tableSize; ++i) {
        rover = hashTable->table[i];
		while (rover != NULL) {
			next = rover->next;
            hashtable_freeEntry(hashTable, rover);
			rover = next;
		}
	}

	/* Free the table */

    free(hashTable->table);

	/* Free the hash table structure */

    free(hashTable);
}

void hashtable_registerFreeFunctions(HashTable *hashTable,
                                     HashTableKeyFreeFunc keyFreeFunc,
                                     HashTableValueFreeFunc valueFreeFunc)
{
    hashTable->keyFreeFunc = keyFreeFunc;
    hashTable->valueFreeFunc = valueFreeFunc;
}


static int hashtable_enlarge(HashTable *hashTable)
{
	/* Store a copy of the old table */

    HashTableEntry **oldTable = hashTable->table;
    unsigned int oldTableSize = hashTable->tableSize;
    unsigned int oldPrimeIndex = hashTable->primeIndex;

	/* Allocate a new, larger table */

    ++hashTable->primeIndex;

    if (!hashtable_allocateTable(hashTable)) {

		/* Failed to allocate the new table */

        hashTable->table = oldTable;
        hashTable->tableSize = oldTableSize;
        hashTable->primeIndex = oldPrimeIndex;

		return 0;
	}

	/* Link all entries from all chains into the new table */

    HashTableEntry *rover;
    HashTablePair *pair;
    HashTableEntry *next;
    unsigned int index;
    unsigned int i;

    for (i=0; i<oldTableSize; ++i) {
        rover = oldTable[i];

		while (rover != NULL) {
			next = rover->next;

			/* Fetch rover HashTablePair */

			pair = &(rover->pair);

			/* Find the index into the new table */

            index = hashTable->hashFunc(pair->key) % hashTable->tableSize;

			/* Link this entry into the chain */

            rover->next = hashTable->table[index];
            hashTable->table[index] = rover;

			/* Advance to next in the chain */

			rover = next;
		}
	}

	/* Free the old table */

    free(oldTable);

	return 1;
}

int hashtable_insert(HashTable *hashTable,
                     HashTableKey key,
                     HashTableValue value)
{
	/* If there are too many items in the table with respect to the table
	 * size, the number of hash collisions increases and performance
	 * decreases. Enlarge the table size to prevent this happening */

    if ((hashTable->entries * 3) / hashTable->tableSize > 0) {

		/* Table is more than 1/3 full */

        if (!hashtable_enlarge(hashTable)) {

			/* Failed to enlarge the table */

			return 0;
		}
	}

	/* Generate the hash of the key and hence the index into the table */

    unsigned int index = hashTable->hashFunc(key) % hashTable->tableSize;

	/* Traverse the chain at this location and look for an existing
	 * entry with the same key */

    HashTableEntry *rover = hashTable->table[index];
    HashTablePair *pair;

	while (rover != NULL) {

		/* Fetch rover's HashTablePair entry */

		pair = &(rover->pair);

        if (hashTable->equalFunc(pair->key, key) != 0) {

			/* Same key: overwrite this entry with new data */

			/* If there is a value free function, free the old data
			 * before adding in the new data */

            if (hashTable->valueFreeFunc != NULL) {
                hashTable->valueFreeFunc(pair->value);
			}

			/* Same with the key: use the new key value and free
			 * the old one */

            if (hashTable->keyFreeFunc != NULL) {
                hashTable->keyFreeFunc(pair->key);
			}

			pair->key = key;
			pair->value = value;

			/* Finished */

			return 1;
		}

		rover = rover->next;
	}

	/* Not in the hash table yet.  Create a new entry */

    HashTableEntry *newEntry = (HashTableEntry *) malloc(sizeof(HashTableEntry));

    if (newEntry == NULL) {
		return 0;
	}

    newEntry->pair.key = key;
    newEntry->pair.value = value;

	/* Link into the list */

    newEntry->next = hashTable->table[index];
    hashTable->table[index] = newEntry;

	/* Maintain the count of the number of entries */

    ++hashTable->entries;

	/* Added successfully */

	return 1;
}

HashTableValue hashtable_lookup(HashTable *hashTable, HashTableKey key)
{
	/* Generate the hash of the key and hence the index into the table */

    unsigned int index = hashTable->hashFunc(key) % hashTable->tableSize;

	/* Walk the chain at this index until the corresponding entry is
	 * found */

    HashTableEntry *rover = hashTable->table[index];
    HashTablePair *pair;

	while (rover != NULL) {
		pair = &(rover->pair);

        if (hashTable->equalFunc(key, pair->key) != 0) {

			/* Found the entry.  Return the data. */

			return pair->value;
		}

		rover = rover->next;
	}

	/* Not found */

	return HASH_TABLE_NULL;
}

int hashtable_remove(HashTable *hashTable, HashTableKey key)
{
	/* Generate the hash of the key and hence the index into the table */

    unsigned int index = hashTable->hashFunc(key) % hashTable->tableSize;

	/* Rover points at the pointer which points at the current entry
	 * in the chain being inspected.  ie. the entry in the table, or
	 * the "next" pointer of the previous entry in the chain.  This
	 * allows us to unlink the entry when we find it. */

    int result = 0;
    HashTableEntry **rover = &hashTable->table[index];
    HashTableEntry *entry;
    HashTablePair *pair;

	while (*rover != NULL) {

		pair = &((*rover)->pair);

        if (hashTable->equalFunc(key, pair->key) != 0) {

			/* This is the entry to remove */

			entry = *rover;

			/* Unlink from the list */

			*rover = entry->next;

			/* Destroy the entry structure */

            hashtable_freeEntry(hashTable, entry);

			/* Track count of entries */

            --hashTable->entries;

			result = 1;

			break;
		}

		/* Advance to the next entry */

		rover = &((*rover)->next);
	}

	return result;
}

unsigned int hashtable_numEntries(HashTable *hashTable)
{
    return hashTable->entries;
}

void hashtable_iterate(HashTable *hashTable, HashTableIterator *iterator)
{
    iterator->hashTable = hashTable;

	/* Default value of next if no entries are found. */

    iterator->nextEntry = NULL;

	/* Find the first entry */

    unsigned int chain;
    for (chain=0; chain<hashTable->tableSize; ++chain) {

        if (hashTable->table[chain] != NULL) {
            iterator->nextEntry = hashTable->table[chain];
            iterator->nextChain = chain;
			break;
		}
	}
}

int hashtable_iteratorHasMore(HashTableIterator *iterator)
{
    return iterator->nextEntry != NULL;
}

HashTablePair hashtable_iteratorNext(HashTableIterator *iterator)
{
    HashTable *hashTable = iterator->hashTable;
    HashTablePair pair = {NULL, NULL};

    if (iterator->nextEntry == NULL) {
		return pair;
	}

	/* Result is immediately available */

    HashTableEntry *currentEntry = iterator->nextEntry;
    pair = currentEntry->pair;

	/* Find the next entry */

    if (currentEntry->next != NULL) {

		/* Next entry in current chain */

        iterator->nextEntry = currentEntry->next;

	} else {

		/* None left in this chain, so advance to the next chain */

        unsigned int chain = iterator->nextChain + 1;

		/* Default value if no next chain found */

        iterator->nextEntry = NULL;

        while (chain < hashTable->tableSize) {

			/* Is there anything in this chain? */

            if (hashTable->table[chain] != NULL) {
                iterator->nextEntry = hashTable->table[chain];
				break;
			}

			/* Try the next chain */

			++chain;
		}

        iterator->nextChain = chain;
	}

	return pair;
}

