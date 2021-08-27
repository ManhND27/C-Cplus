#include <stdlib.h>

#include "dssinglylinkedlist.h"


/* A singly-linked list */

struct _SListEntry {
    SListValue data;
    SListEntry *next;
};

void slist_free(SListEntry *list)
{
	/* Iterate over each entry, freeing each list entry, until the
	 * end is reached */

    SListEntry *entry = list;

	while (entry != NULL) {
		SListEntry *next;

		next = entry->next;

		free(entry);

		entry = next;
	}
}

SListEntry *slist_prepend(SListEntry **list, SListValue data)
{
	/* Create new entry */

    SListEntry *newEntry = malloc(sizeof(SListEntry));

    if (newEntry == NULL) {
		return NULL;
	}

    newEntry->data = data;

	/* Hook into the list start */

    newEntry->next = *list;
    *list = newEntry;

    return newEntry;
}

SListEntry *slist_append(SListEntry **list, SListValue data)
{
	/* Create new list entry */

    SListEntry *newEntry = malloc(sizeof(SListEntry));

    if (newEntry == NULL) {
		return NULL;
	}

    newEntry->data = data;
    newEntry->next = NULL;

	/* Hooking into the list is different if the list is empty */

    SListEntry *rover;
	if (*list == NULL) {

		/* Create the start of the list */

        *list = newEntry;

	} else {

		/* Find the end of list */

		for (rover=*list; rover->next != NULL; rover = rover->next);

		/* Add to the end of list */

        rover->next = newEntry;
	}

    return newEntry;
}

SListValue slist_data(SListEntry *listentry)
{
	return listentry->data;
}

void slist_setData(SListEntry *entry, SListValue data)
{
    if (entry != NULL) {
        entry->data = data;
	}
}

SListEntry *slist_next(SListEntry *listentry)
{
	return listentry->next;
}

SListEntry *slist_nthEntry(SListEntry *list, unsigned int n)
{
	/* Iterate through n list entries to reach the desired entry.
	 * Make sure we do not reach the end of the list. */

    SListEntry *entry = list;
    unsigned int i;

	for (i=0; i<n; ++i) {

		if (entry == NULL) {
			return NULL;
		}
		entry = entry->next;
	}

	return entry;
}

SListValue slist_nthData(SListEntry *list, unsigned int n)
{
	/* Find the specified entry */

    SListEntry *entry = slist_nthEntry(list, n);

	/* If out of range, return NULL, otherwise return the data */

	if (entry == NULL) {
		return SLIST_NULL;
	} else {
		return entry->data;
	}
}

unsigned int slist_length(SListEntry *list)
{
    SListEntry *entry = list;
    unsigned int length = 0;

	while (entry != NULL) {

		/* Count the number of entries */

		++length;

		entry = entry->next;
	}

	return length;
}

SListValue *slist_toArray(SListEntry *list)
{
	/* Allocate an array equal in size to the list length */

    unsigned int length = slist_length(list);

    SListValue *array = malloc(sizeof(SListValue) * length);

	if (array == NULL) {
		return NULL;
	}

	/* Add all entries to the array */

    SListEntry *rover = list;
    unsigned int i;

	for (i=0; i<length; ++i) {

		/* Add this node's data */

		array[i] = rover->data;

		/* Jump to the next list node */

		rover = rover->next;
	}

	return array;
}

int slist_removeEntry(SListEntry **list, SListEntry *entry)
{
	/* If the list is empty, or entry is NULL, always fail */

	if (*list == NULL || entry == NULL) {
		return 0;
	}

	/* Action to take is different if the entry is the first in the list */

    SListEntry *rover;

	if (*list == entry) {

		/* Unlink the first entry and update the starting pointer */

		*list = entry->next;

	} else {

		/* Search through the list to find the preceding entry */

		rover = *list;

		while (rover != NULL && rover->next != entry) {
			rover = rover->next;
		}

		if (rover == NULL) {

			/* Not found in list */

			return 0;

		} else {

			/* rover->next now points at entry, so rover is the
			 * preceding entry. Unlink the entry from the list. */

			rover->next = entry->next;
		}
	}

	/* Free the list entry */

	free(entry);

	/* Operation successful */

	return 1;
}

unsigned int slist_removeData(SListEntry **list,
                              SListEqualFunc callback,
                              SListValue data)
{
	/* Iterate over the list.  'rover' points at the entrypoint into the
	 * current entry, ie. the list variable for the first entry in the
	 * list, or the "next" field of the preceding entry. */

    SListEntry **rover = list;
    SListEntry *next;
    unsigned int entriesRemoved = 0;

	while (*rover != NULL) {

		/* Should this entry be removed? */

		if (callback((*rover)->data, data) != 0) {

			/* Data found, so remove this entry and free */

			next = (*rover)->next;
			free(*rover);
			*rover = next;

			/* Count the number of entries removed */

            ++entriesRemoved;
		} else {

			/* Advance to the next entry */

			rover = &((*rover)->next);
		}
	}

    return entriesRemoved;
}

/* Function used internally for sorting.  Returns the last entry in the
 * new sorted list */

static SListEntry *slist_sortInternal(SListEntry **list, SListCompareFunc compareFunc)
{
	/* If there are less than two entries in this list, it is
	 * already sorted */

	if (*list == NULL || (*list)->next == NULL) {
		return *list;
	}

	/* The first entry is the pivot */

    SListEntry *pivot = *list;

	/* Iterate over the list, starting from the second entry.  Sort
	 * all entries into the less and more lists based on comparisons
	 * with the pivot */

    SListEntry *lessList = NULL;
    SListEntry *moreList = NULL;
    SListEntry *rover = (*list)->next;

	while (rover != NULL) {
		SListEntry *next = rover->next;

        if (compareFunc(rover->data, pivot->data) < 0) {

			/* Place this in the less list */

            rover->next = lessList;
            lessList = rover;

		} else {

			/* Place this in the more list */

            rover->next = moreList;
            moreList = rover;

		}

		rover = next;
	}

	/* Sort the sublists recursively */

    SListEntry *lessListEnd = slist_sortInternal(&lessList, compareFunc);
    SListEntry *moreListEnd = slist_sortInternal(&moreList, compareFunc);

	/* Create the new list starting from the less list */

    *list = lessList;

	/* Append the pivot to the end of the less list.  If the less list
	 * was empty, start from the pivot */

    if (lessList == NULL) {
		*list = pivot;
	} else {
        lessListEnd->next = pivot;
	}

	/* Append the more list after the pivot */

    pivot->next = moreList;

	/* Work out what the last entry in the list is.  If the more list was
	 * empty, the pivot was the last entry.  Otherwise, the end of the
	 * more list is the end of the total list. */

    if (moreList == NULL) {
		return pivot;
	} else {
        return moreListEnd;
	}
}

void slist_sort(SListEntry **list, SListCompareFunc compareFunc)
{
    slist_sortInternal(list, compareFunc);
}

SListEntry *slist_findData(SListEntry *list,
                           SListEqualFunc callback,
                           SListValue data)
{
	SListEntry *rover;

	/* Iterate over entries in the list until the data is found */

	for (rover=list; rover != NULL; rover=rover->next) {
		if (callback(rover->data, data) != 0) {
			return rover;
		}
	}

	/* Not found */

	return NULL;
}

void slist_iterate(SListEntry **list, SListIterator *iterator)
{
	/* Start iterating from the beginning of the list. */

    iterator->prevNext = list;

	/* We have not yet read the first item. */

    iterator->current = NULL;
}

int slist_iteratorHasMore(SListIterator *iterator)
{
    if (iterator->current == NULL || iterator->current != *iterator->prevNext) {

		/* Either we have not read the first entry, the current
		 * item was removed or we have reached the end of the
         * list.  Use prevNext to determine if we have a next
		 * value to iterate over. */

        return *iterator->prevNext != NULL;

	} else {

		/* The current entry has not been deleted.  There
		 * is a next entry if current->next is not NULL. */

        return iterator->current->next != NULL;
	}
}

SListValue slist_iteratorNext(SListIterator *iterator)
{
    if (iterator->current == NULL || iterator->current != *iterator->prevNext) {

		/* Either we are reading the first entry, we have reached
		 * the end of the list, or the previous entry was removed.
         * Get the next entry with iter->prevNext. */

        iterator->current = *iterator->prevNext;

	} else {

		/* Last value returned from slist_iter_next was not
		 * deleted. Advance to the next entry. */

        iterator->prevNext = &iterator->current->next;
        iterator->current = iterator->current->next;
	}

	/* Have we reached the end of the list? */

    if (iterator->current == NULL) {
		return SLIST_NULL;
	} else {
        return iterator->current->data;
	}
}

void slist_iteratorRemove(SListIterator *iterator)
{
    if (iterator->current == NULL || iterator->current != *iterator->prevNext) {

		/* Either we have not yet read the first item, we have
		 * reached the end of the list, or we have already removed
		 * the current value.  Either way, do nothing. */

	} else {

		/* Remove the current entry */

        *iterator->prevNext = iterator->current->next;
        free(iterator->current);
        iterator->current = NULL;
	}
}

