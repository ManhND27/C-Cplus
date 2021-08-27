#include <stdlib.h>

#include "dslist.h"


/* A doubly-linked list */

struct _ListEntry {
	ListValue data;
	ListEntry *prev;
	ListEntry *next;
};

void list_free(ListEntry *list)
{
	/* Iterate over each entry, freeing each list entry, until the
	 * end is reached */

    ListEntry *entry = list;

	while (entry != NULL) {
		ListEntry *next;

		next = entry->next;

		free(entry);

		entry = next;
	}
}

ListEntry *list_prepend(ListEntry **list, ListValue data)
{
	if (list == NULL) {

		/* not a valid list */

		return NULL;
	}

	/* Create new entry */

    ListEntry *newEntry = malloc(sizeof(ListEntry));
    if (newEntry == NULL) {
		return NULL;
	}

    newEntry->data = data;

	/* Hook into the list start */

	if (*list != NULL) {
        (*list)->prev = newEntry;
	}
    newEntry->prev = NULL;
    newEntry->next = *list;
    *list = newEntry;

    return newEntry;
}

ListEntry *list_append(ListEntry **list, ListValue data)
{
	if (list == NULL) {
		return NULL;
	}

	/* Create new list entry */

    ListEntry *newEntry = malloc(sizeof(ListEntry));
    if (newEntry == NULL) {
		return NULL;
	}

    newEntry->data = data;
    newEntry->next = NULL;

	/* Hooking into the list is different if the list is empty */

    ListEntry *rover;

	if (*list == NULL) {

		/* Create the start of the list */

        *list = newEntry;
        newEntry->prev = NULL;

	} else {

		/* Find the end of list */

		for (rover=*list; rover->next != NULL; rover = rover->next);

		/* Add to the end of list */

        newEntry->prev = rover;
        rover->next = newEntry;
	}

    return newEntry;
}

ListValue list_data(ListEntry *entry)
{
    if (entry == NULL) {
		return LIST_NULL;
	}

    return entry->data;
}

void list_setData(ListEntry *entry, ListValue value)
{
    if (entry != NULL) {
        entry->data = value;
	}
}

ListEntry *list_prev(ListEntry *entry)
{
    if (entry == NULL) {
		return NULL;
	}

    return entry->prev;
}

ListEntry *list_next(ListEntry *entry)
{
    if (entry == NULL) {
		return NULL;
	}

    return entry->next;
}

ListEntry *list_nthEntry(ListEntry *list, unsigned int n)
{
	/* Iterate through n list entries to reach the desired entry.
	 * Make sure we do not reach the end of the list. */

    ListEntry *entry = list;
    unsigned int i;

	for (i=0; i<n; ++i) {

		if (entry == NULL) {
			return NULL;
		}
		entry = entry->next;
	}

	return entry;
}

ListValue list_nthData(ListEntry *list, unsigned int n)
{
	/* Find the specified entry */

    ListEntry *entry = list_nthEntry(list, n);

	/* If out of range, return NULL, otherwise return the data */

	if (entry == NULL) {
		return LIST_NULL;
	} else {
		return entry->data;
	}
}

unsigned int list_length(ListEntry *list)
{
    unsigned int length = 0;
    ListEntry *entry = list;

	while (entry != NULL) {

		/* Count the number of entries */

		++length;

		entry = entry->next;
	}

	return length;
}

ListValue *list_toArray(ListEntry *list)
{
	/* Allocate an array equal in size to the list length */

    unsigned int length = list_length(list);

    ListValue *array = malloc(sizeof(ListValue) * length);

	if (array == NULL) {
		return NULL;
	}

	/* Add all entries to the array */

    ListEntry *rover = list;

    unsigned int i;

	for (i=0; i<length; ++i) {

		/* Add this node's data */

		array[i] = rover->data;

		/* Jump to the next list node */

		rover = rover->next;
	}

	return array;
}

int list_removeEntry(ListEntry **list, ListEntry *entry)
{
	/* If the list is empty, or entry is NULL, always fail */

	if (list == NULL || *list == NULL || entry == NULL) {
		return 0;
	}

	/* Action to take is different if the entry is the first in the list */

	if (entry->prev == NULL) {

		/* Unlink the first entry and update the starting pointer */

		*list = entry->next;

		/* Update the second entry's prev pointer, if there is a second
		 * entry */

		if (entry->next != NULL) {
			entry->next->prev = NULL;
		}

	} else {

		/* This is not the first in the list, so we must have a
		 * previous entry.  Update its 'next' pointer to the new
		 * value */

		entry->prev->next = entry->next;

		/* If there is an entry following this one, update its 'prev'
		 * pointer to the new value */

		if (entry->next != NULL) {
			entry->next->prev = entry->prev;
		}
	}

	/* Free the list entry */

	free(entry);

	/* Operation successful */

	return 1;
}

unsigned int list_removeData(ListEntry **list,
                             ListEqualFunc callback,
                             ListValue data)
{
	if (list == NULL || callback == NULL) {
		return 0;
	}

    unsigned int entriesRemoved = 0;

	/* Iterate over the entries in the list */

    ListEntry *rover = *list;
    ListEntry *next;

	while (rover != NULL) {

		next = rover->next;

		if (callback(rover->data, data)) {

			/* This data needs to be removed.  Unlink this entry
			 * from the list. */

			if (rover->prev == NULL) {

				/* This is the first entry in the list */

				*list = rover->next;
			} else {

				/* Point the previous entry at its new
				 * location */

				rover->prev->next = rover->next;
			}

			if (rover->next != NULL) {
				rover->next->prev = rover->prev;
			}

			/* Free the entry */

			free(rover);

            ++entriesRemoved;
		}

		/* Advance to the next list entry */

		rover = next;
	}

    return entriesRemoved;
}

/* Function used internally for sorting.  Returns the last entry in the
 * new sorted list */

static ListEntry *list_sortInternal(ListEntry **list,
                                    ListCompareFunc compareFunc)
{
    if (list == NULL || compareFunc == NULL) {
		return NULL;
	}

	/* If there are less than two entries in this list, it is
	 * already sorted */

	if (*list == NULL || (*list)->next == NULL) {
		return *list;
	}

	/* The first entry is the pivot */

    ListEntry *pivot = *list;

	/* Iterate over the list, starting from the second entry.  Sort
	 * all entries into the less and more lists based on comparisons
	 * with the pivot */

    ListEntry *lessList = NULL;
    ListEntry *moreList = NULL;
    ListEntry *rover = (*list)->next;

	while (rover != NULL) {
		ListEntry *next = rover->next;

        if (compareFunc(rover->data, pivot->data) < 0) {

			/* Place this in the less list */

			rover->prev = NULL;
            rover->next = lessList;
            if (lessList != NULL) {
                lessList->prev = rover;
			}
            lessList = rover;

		} else {

			/* Place this in the more list */

			rover->prev = NULL;
            rover->next = moreList;
            if (moreList != NULL) {
                moreList->prev = rover;
			}
            moreList = rover;
		}

		rover = next;
	}

	/* Sort the sublists recursively */

    ListEntry *lessListEnd = list_sortInternal(&lessList, compareFunc);
    ListEntry *moreListEnd = list_sortInternal(&moreList, compareFunc);

	/* Create the new list starting from the less list */

    *list = lessList;

	/* Append the pivot to the end of the less list.  If the less list
	 * was empty, start from the pivot */

    if (lessList == NULL) {
		pivot->prev = NULL;
		*list = pivot;
	} else {
        pivot->prev = lessListEnd;
        lessListEnd->next = pivot;
	}

	/* Append the more list after the pivot */

    pivot->next = moreList;
    if (moreList != NULL) {
        moreList->prev = pivot;
	}

	/* Work out what the last entry in the list is.  If the more list was
	 * empty, the pivot was the last entry.  Otherwise, the end of the
	 * more list is the end of the total list. */

    if (moreList == NULL) {
		return pivot;
	} else {
        return moreListEnd;
	}
}

void list_sort(ListEntry **list, ListCompareFunc compareFunc)
{
    list_sortInternal(list, compareFunc);
}

ListEntry *list_findData(ListEntry *list,
                         ListEqualFunc callback,
                         ListValue data)
{
	/* Iterate over entries in the list until the data is found */

    ListEntry *rover;

	for (rover=list; rover != NULL; rover=rover->next) {
		if (callback(rover->data, data) != 0) {
			return rover;
		}
	}

	/* Not found */

	return NULL;
}

void list_iterate(ListEntry **list, ListIterator *iterator)
{
	/* Start iterating from the beginning of the list. */

    iterator->prevNext = list;

	/* We have not yet read the first item. */

    iterator->current = NULL;
}

int list_iteratorHasMore(ListIterator *iterator)
{
    if (iterator->current == NULL || iterator->current != *iterator->prevNext) {

		/* Either we have not read the first entry, the current
		 * item was removed or we have reached the end of the
		 * list.  Use prev_next to determine if we have a next
		 * value to iterate over. */

        return *iterator->prevNext != NULL;

	} else {
		/* The current entry as not been deleted since the last
		 * call to list_iter_next: there is a next entry if
		 * current->next is not NULL */

        return iterator->current->next != NULL;
	}
}

ListValue list_iteratorNext(ListIterator *iterator)
{
    if (iterator->current == NULL || iterator->current != *iterator->prevNext) {

		/* Either we are reading the first entry, we have reached
		 * the end of the list, or the previous entry was removed.
         * Get the next entry with iter->prevNext. */

        iterator->current = *iterator->prevNext;

	} else {

		/* Last value returned from list_iter_next was not deleted.
		 * Advance to the next entry. */

        iterator->prevNext = &iterator->current->next;
        iterator->current = iterator->current->next;
	}

	/* Have we reached the end of the list? */

    if (iterator->current == NULL) {
		return LIST_NULL;
	} else {
        return iterator->current->data;
	}
}

void list_iteratorRemove(ListIterator *iterator)
{
    if (iterator->current == NULL || iterator->current != *iterator->prevNext) {

		/* Either we have not yet read the first item, we have
		 * reached the end of the list, or we have already removed
		 * the current value.  Either way, do nothing. */

	} else {

		/* Remove the current entry */

        *iterator->prevNext = iterator->current->next;

        if (iterator->current->next != NULL) {
            iterator->current->next->prev = iterator->current->prev;
		}

        free(iterator->current);
        iterator->current = NULL;
	}
}

