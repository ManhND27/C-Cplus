/**
 * @file sortedarray.c
 * 
 * @brief File containing the implementation of sortedarray.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dssortedarray.h"


/**
 * Definition of a @ref SortedArray
 */
struct _SortedArray {
	/**
	 * This field contains the actual array. The array always has a length
	 * of value of field length.
	 */
	SortedArrayValue *data;

	/**
	 * The length of the sorted array.
	 */
	unsigned int length;

	/**
	 * Field for internal usage only indicating how much memory already has
	 * been allocated for *data.
	 */
	unsigned int _alloced;

	/**
	 * The callback used to determine if two values equal.
	 */
    SortedArrayEqualFunc equalFunc;

	/**
	 * The callback use to determine the order of two values.
	 */
    SortedArrayCompareFunc compareFunc;
};

/* Function for finding first index of range which equals data. An equal value
   must be present. */
static unsigned int sortedarray_firstIndex(SortedArray *sortedarray,
                                           SortedArrayValue data,
                                           unsigned int left,
                                           unsigned int right)
{
	unsigned int index = left;

	while (left < right) {
		index = (left + right) / 2;

        int order = sortedarray->compareFunc(data, sortedarray->data[index]);
		if (order > 0) {
			left = index + 1;
		} else {
			right = index;
		}
	}

	return index;
}

/* Function for finding last index of range which equals data. An equal value
   must be present. */
static unsigned int sortedarray_lastIndex(SortedArray *sortedarray,
                                          SortedArrayValue data,
                                          unsigned int left,
                                          unsigned int right)
{
	unsigned int index = right;

	while (left < right) {
		index = (left + right) / 2;

        int order = sortedarray->compareFunc(data, sortedarray->data[index]);
		if (order <= 0) {
			left = index + 1;
		} else {
			right = index;
		}
	}

	return index;
}

SortedArrayValue *sortedarray_get(SortedArray *array, unsigned int i)
{
	//check if array is NULL
	if (array == NULL) {
		return NULL;
	}

	//otherwise just return the element
	return array->data[i];	
}

unsigned int sortedarray_length(SortedArray *array)
{
	return array->length;
}

SortedArray *sortedarray_new(unsigned int length,
                             SortedArrayEqualFunc equalFunc,
                             SortedArrayCompareFunc compareFunc)
{
	/* check input requirements */
    if (equalFunc == NULL || compareFunc == NULL) {
		return NULL;
	}

	/* If length is 0, set it to a default. */
	if (length == 0) {
		length = 16;
	}

	SortedArrayValue *array = malloc(sizeof(SortedArrayValue) * length);

	/* on failure, return null */
	if (array == NULL) {
		return NULL;
	}

	SortedArray *sortedarray = malloc(sizeof(SortedArray));    

	/* check for failure */
	if (sortedarray == NULL) {
		free(array);
		return NULL;
	}
    
	/* init */
	sortedarray->data = array;
	sortedarray->length = 0;
	sortedarray->_alloced = length;
    sortedarray->equalFunc = equalFunc;
    sortedarray->compareFunc = compareFunc;
	return sortedarray;
}

void sortedarray_free(SortedArray *sortedarray)
{
	if (sortedarray != NULL) {
		free(sortedarray->data);
		free(sortedarray);
	}
}

void sortedarray_remove(SortedArray *sortedarray, unsigned int index)
{
	/* same as remove range of length 1 */
    sortedarray_removeRange(sortedarray, index, 1);
}

void sortedarray_removeRange(SortedArray *sortedArray,
                             unsigned int index,
                             unsigned int length)
{
	/* removal does not violate sorted property */

	/* check if valid range */
    if (index > sortedArray->length || index + length > sortedArray->length) {
		return;
	}

	/* move entries back */
    memmove(&sortedArray->data[index],
            &sortedArray->data[index + length],
            (sortedArray->length - (index + length))
	              * sizeof(SortedArrayValue));

    sortedArray->length -= length;
}

int sortedarray_insert(SortedArray *sortedArray, SortedArrayValue data)
{
	/* do a binary search like loop to find right position */
	unsigned int left  = 0;
    unsigned int right = sortedArray->length;
	unsigned int index = 0;

	/* When length is 1 set right to 0 so that the loop is not entered */	
	right = (right > 1) ? right : 0;

	while (left != right) {
		index = (left + right) / 2;

        int order = sortedArray->compareFunc(data, sortedArray->data[index]);
		if (order < 0) {
			/* value should be left of index */
			right = index;
		} else if (order > 0) {
			/* value should be right of index */
			left = index + 1;
		} else {
			/* value should be at index */
			break;
		}
	}

	/* look whether the item should be put before or after the index */
    if (sortedArray->length > 0
            && sortedArray->compareFunc(data, sortedArray->data[index]) > 0) {
		index++;
	}

	/* insert element at index */
    if (sortedArray->length + 1 > sortedArray->_alloced) {
		/* enlarge the array */
        unsigned int newSize;
		SortedArrayValue *data;

        newSize = sortedArray->_alloced * 2;
        data = realloc(sortedArray->data, sizeof(SortedArrayValue) * newSize);

		if (data == NULL) {
			return 0;
		} else {
            sortedArray->data = data;
            sortedArray->_alloced = newSize;
		}
	}

	/* move all other elements */
    memmove(&sortedArray->data[index + 1],
            &sortedArray->data[index],
            (sortedArray->length - index) * sizeof(SortedArrayValue));

	/* insert entry */
    sortedArray->data[index] = data;
    ++(sortedArray->length);

	return 1;
}

int sortedarray_indexOf(SortedArray *sortedArray, SortedArrayValue data)
{
    if (sortedArray == NULL) {
		return -1;
	}
	
	/* do a binary search */
	unsigned int left = 0;
    unsigned int right = sortedArray->length;
	unsigned int index = 0;

	/* safe subtract 1 of right without going negative */
	right = (right > 1) ? right : 0;

	while (left != right) {
		index = (left + right) / 2;

        int order = sortedArray->compareFunc(data, sortedArray->data[index]);
		if (order < 0) {
			/* value should be left */
			right = index;
		} else if (order > 0) {
			/* value should be right */
			left = index + 1;
		} else {
			/* no binary search can be done anymore, 
			   search linear now */
            left = sortedarray_firstIndex(sortedArray, data, left,
			                               index);
            right = sortedarray_lastIndex(sortedArray, data,
			                               index, right);

			for (index = left; index <= right; index++) {
                if (sortedArray->equalFunc(data, sortedArray->data[index])) {
					return (int) index;
				}
			}

			/* nothing is found */
			return -1;
		}
	}

	return -1;
}

void sortedarray_clear(SortedArray *sortedarray)
{
	/* set length to 0 */
	sortedarray->length = 0;
}
