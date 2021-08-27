#include <stdlib.h>
#include <string.h>
#include "dsset.h"


/* A set */

struct _SetEntry {
	SetValue data;
	SetEntry *next;
};

struct _Set {
	SetEntry **table;
	unsigned int entries;
    unsigned int tableSize;
    unsigned int primeIndex;
    SetHashFunc hashFunc;
    SetEqualFunc equalFunc;
    SetFreeFunc freeFunc;
};

/* This is a set of good hash table prime numbers, from:
 *   http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
 * Each prime is roughly double the previous value, and as far as
 * possible from the nearest powers of two. */

static const unsigned int SET_PRIMES[] = {
	193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
	196613, 393241, 786433, 1572869, 3145739, 6291469,
	12582917, 25165843, 50331653, 100663319, 201326611,
	402653189, 805306457, 1610612741,
};

static const unsigned int SET_NUM_PRIMES = sizeof(SET_PRIMES) / sizeof(int);

static int set_allocateTable(Set *set)
{
	/* Determine the table size based on the current prime index.
	 * An attempt is made here to ensure sensible behavior if the
	 * maximum prime is exceeded, but in practice other things are
	 * likely to break long before that happens. */

    if (set->primeIndex < SET_NUM_PRIMES) {
        set->tableSize = SET_PRIMES[set->primeIndex];
	} else {
        set->tableSize = set->entries * 10;
	}

	/* Allocate the table and initialise to NULL */

    set->table = calloc(set->tableSize, sizeof(SetEntry *));

	return set->table != NULL;
}

static void set_freeEntry(Set *set, SetEntry *entry)
{
	/* If there is a free function registered, call it to free the
	 * data for this entry first */

    if (set->freeFunc != NULL) {
        set->freeFunc(entry->data);
	}

	/* Free the entry structure */

	free(entry);
}

Set *set_new(SetHashFunc hashFunc, SetEqualFunc equalFunc)
{
	/* Allocate a new set and fill in the fields */

    Set *newSet = (Set *) malloc(sizeof(Set));
    if (newSet == NULL) {
		return NULL;
	}

    newSet->hashFunc = hashFunc;
    newSet->equalFunc = equalFunc;
    newSet->entries = 0;
    newSet->primeIndex = 0;
    newSet->freeFunc = NULL;

	/* Allocate the table */

    if (!set_allocateTable(newSet)) {
        free(newSet);
		return NULL;
	}

    return newSet;
}

void set_free(Set *set)
{
	SetEntry *rover;
	SetEntry *next;
	unsigned int i;

	/* Free all entries in all chains */

    for (i=0; i<set->tableSize; ++i) {
		rover = set->table[i];

		while (rover != NULL) {
			next = rover->next;

			/* Free this entry */

            set_freeEntry(set, rover);

			/* Advance to the next entry in the chain */

			rover = next;
		}
	}

	/* Free the table */

	free(set->table);

	/* Free the set structure */

	free(set);
}

void set_registerFreeFunction(Set *set, SetFreeFunc freeFunc)
{
    set->freeFunc = freeFunc;
}

static int set_enlarge(Set *set)
{
	/* Store the old table */

    SetEntry **oldTable = set->table;
    unsigned int oldTableSize = set->tableSize;
    unsigned int oldPrimeIndex = set->primeIndex;

	/* Use the next table size from the prime number array */

    ++set->primeIndex;

	/* Allocate the new table */

    if (!set_allocateTable(set)) {
        set->table = oldTable;
        set->tableSize = oldTableSize;
        set->primeIndex = oldPrimeIndex;

		return 0;
	}

	/* Iterate through all entries in the old table and add them
	 * to the new one */

    SetEntry *rover;
    SetEntry *next;
    unsigned int index;
    unsigned int i;

    for (i=0; i<oldTableSize; ++i) {

		/* Walk along this chain */

        rover = oldTable[i];

		while (rover != NULL) {

			next = rover->next;

			/* Hook this entry into the new table */

            index = set->hashFunc(rover->data) % set->tableSize;
			rover->next = set->table[index];
			set->table[index] = rover;

			/* Advance to the next entry in the chain */

			rover = next;
		}
	}

	/* Free back the old table */

    free(oldTable);

	/* Resized successfully */

	return 1;
}

int set_insert(Set *set, SetValue data)
{
	/* The hash table becomes less efficient as the number of entries
	 * increases. Check if the percentage used becomes large. */

    if ((set->entries * 3) / set->tableSize > 0) {

		/* The table is more than 1/3 full and must be increased
		 * in size */

		if (!set_enlarge(set)) {
			return 0;
		}
	}

	/* Use the hash of the data to determine an index to insert into the
	 * table at. */

    unsigned int index = set->hashFunc(data) % set->tableSize;

	/* Walk along this chain and attempt to determine if this data has
	 * already been added to the table */

    SetEntry *rover = set->table[index];

	while (rover != NULL) {

        if (set->equalFunc(data, rover->data) != 0) {

			/* This data is already in the set */

			return 0;
		}

		rover = rover->next;
	}

	/* Not in the set.  We must add a new entry. */

	/* Make a new entry for this data */

    SetEntry *newEntry = (SetEntry *) malloc(sizeof(SetEntry));

    if (newEntry == NULL) {
		return 0;
	}

    newEntry->data = data;

	/* Link into chain */

    newEntry->next = set->table[index];
    set->table[index] = newEntry;

	/* Keep track of the number of entries in the set */

	++set->entries;

	/* Added successfully */

	return 1;
}

int set_remove(Set *set, SetValue data)
{
	/* Look up the data by its hash key */

    unsigned int index = set->hashFunc(data) % set->tableSize;

	/* Search this chain, until the corresponding entry is found */

    SetEntry **rover = &set->table[index];
    SetEntry *entry;

	while (*rover != NULL) {
        if (set->equalFunc(data, (*rover)->data) != 0) {

			/* Found the entry */

			entry = *rover;

			/* Unlink from the linked list */

			*rover = entry->next;

			/* Update counter */

			--set->entries;

			/* Free the entry and return */

            set_freeEntry(set, entry);

			return 1;
		}

		/* Advance to the next entry */

		rover = &((*rover)->next);
	}

	/* Not found in set */

	return 0;
}

int set_query(Set *set, SetValue data)
{
	/* Look up the data by its hash key */

    unsigned int index = set->hashFunc(data) % set->tableSize;

	/* Search this chain, until the corresponding entry is found */

    SetEntry *rover = set->table[index];

	while (rover != NULL) {
        if (set->equalFunc(data, rover->data) != 0) {

			/* Found the entry */

			return 1;
		}

		/* Advance to the next entry in the chain */

		rover = rover->next;
	}

	/* Not found */

	return 0;
}

unsigned int set_numEntries(Set *set)
{
	return set->entries;
}

SetValue *set_toArray(Set *set)
{
	/* Create an array to hold the set entries */

    SetValue *array = malloc(sizeof(SetValue) * set->entries);

	if (array == NULL) {
		return NULL;
	}

    int arrayCounter = 0;

	/* Iterate over all entries in all chains */

    SetEntry *rover;
    unsigned int i;

    for (i=0; i<set->tableSize; ++i) {

		rover = set->table[i];

		while (rover != NULL) {

			/* Add this value to the array */

            array[arrayCounter] = rover->data;
            ++arrayCounter;

			/* Advance to the next entry */

			rover = rover->next;
		}
	}

	return array;
}

Set *set_union(Set *set1, Set *set2)
{
    Set *newSet = set_new(set1->hashFunc, set1->equalFunc);
    if (newSet == NULL) {
		return NULL;
	}

	/* Add all values from the first set */

    SetIterator iterator;
	set_iterate(set1, &iterator);

    SetValue value;

    while (set_iteratorHasMore(&iterator)) {

		/* Read the next value */

        value = set_iteratorNext(&iterator);

		/* Copy the value into the new set */

        if (!set_insert(newSet, value)) {

			/* Failed to insert */

            set_free(newSet);
			return NULL;
		}
	}

	/* Add all values from the second set */

	set_iterate(set2, &iterator);

    while (set_iteratorHasMore(&iterator)) {

		/* Read the next value */

        value = set_iteratorNext(&iterator);

		/* Has this value been put into the new set already?
		 * If so, do not insert this again */

        if (set_query(newSet, value) == 0) {
            if (!set_insert(newSet, value)) {

				/* Failed to insert */

                set_free(newSet);
				return NULL;
			}
		}
	}

    return newSet;
}

Set *set_intersection(Set *set1, Set *set2)
{
    Set *newSet = set_new(set1->hashFunc, set2->equalFunc);
    if (newSet == NULL) {
		return NULL;
	}

	/* Iterate over all values in set 1. */

    SetIterator iterator;
	set_iterate(set1, &iterator);

    SetValue value;

    while (set_iteratorHasMore(&iterator)) {

		/* Get the next value */

        value = set_iteratorNext(&iterator);

		/* Is this value in set 2 as well?  If so, it should be
		 * in the new set. */

		if (set_query(set2, value) != 0) {

			/* Copy the value first before inserting,
			 * if necessary */

            if (!set_insert(newSet, value)) {
                set_free(newSet);

				return NULL;
			}
		}
	}

    return newSet;
}

void set_iterate(Set *set, SetIterator *iterator)
{
    iterator->set = set;
    iterator->nextEntry = NULL;

	/* Find the first entry */

    unsigned int chain;
    for (chain = 0; chain < set->tableSize; ++chain) {

		/* There is a value at the start of this chain */

		if (set->table[chain] != NULL) {
            iterator->nextEntry = set->table[chain];
			break;
		}
	}

    iterator->nextChain = chain;
}

SetValue set_iteratorNext(SetIterator *iterator)
{
    Set *set = iterator->set;

	/* No more entries? */

	if (iterator->nextEntry == NULL) {
		return SET_NULL;
	}

	/* We have the result immediately */

    SetEntry *currentEntry = iterator->nextEntry;
    SetValue result = currentEntry->data;

	/* Advance next_entry to the next SetEntry in the Set. */

    unsigned int chain;

    if (currentEntry->next != NULL) {

		/* Use the next value in this chain */

        iterator->nextEntry = currentEntry->next;

	} else {

		/* Default value if no valid chain is found */

		iterator->nextEntry = NULL;

		/* No more entries in this chain.  Search the next chain */

		chain = iterator->nextChain + 1;

        while (chain < set->tableSize) {

			/* Is there a chain at this table entry? */

			if (set->table[chain] != NULL) {

				/* Valid chain found! */

				iterator->nextEntry = set->table[chain];

				break;
			}

			/* Keep searching until we find an empty chain */

			++chain;
		}

		iterator->nextChain = chain;
	}

	return result;
}

int set_iteratorHasMore(SetIterator *iterator)
{
	return iterator->nextEntry != NULL;
}

