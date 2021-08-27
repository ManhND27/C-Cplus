#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dsarraylist.h"

void printArrayList(ArrayList *arraylist)
{
    for (int i = 0; i < arraylist->length; ++i) {
        printf("%d ",arraylist->data[i]);
    }
}

int *EqualFunc(ArrayListValue value1, ArrayListValue value2)
{
    if (value1 != value2){
        return 0;
    }else
    return 1;
}

int *SortUpFunc(ArrayListValue value1, ArrayListValue value2){
    if (value1 > value2) {
        return 1;
    }else if (value1 < value2){
        return -1;
    }else {
        return 0;
    }
}

int *SortDownFunc(ArrayListValue value1, ArrayListValue value2){
    if (value1 > value2) {
        return -1;
    }else if (value1 < value2){
        return 1;
    }else {
        return 0;
    }
}

int main()
{
    int a = 5;
    ArrayList arrayList1 = *arraylist_new(a);
    ArrayList arrayList2 = *arraylist_new(a);
    printf("%d \n", arrayList1.length);
    int x = 1;
    int y = 2;
    int z = 3;

    arraylist_append(&arrayList1, x);
    arraylist_append(&arrayList1, y);
    arraylist_append(&arrayList1, z);
    printArrayList(&arrayList1);
    printf("\nlength arraylist : %d \n", arrayList1.length);
    printf("alloced arraylist : %d \n", arrayList1._alloced);
    printf("\n");

    arraylist_prepend(&arrayList2, x);
    arraylist_prepend(&arrayList2, y);
    arraylist_prepend(&arrayList2, z);
    printArrayList(&arrayList2);
    //arraylist_free(&arrayList2);
    printf("\nlength arraylist : %d \n", arrayList2.length);
    printf("alloced arraylist : %d \n", arrayList2._alloced);

    printf("\nInsert 4 on index arraylist1[2] : ");
    arraylist_insert(&arrayList1, 2, 4);
    printArrayList(&arrayList1);

    printf("\nRemove 4 on index arraylist1[2] : ");
    arraylist_remove(&arrayList1, 2);
    printArrayList(&arrayList1);

    int b = 3;
    int indexB = arraylist_indexOf(&arrayList1, &EqualFunc, b);
    printf("\nIndex number b on arraylist1 : %d",indexB);

    arraylist_clear(&arrayList2);
    printf("\nprint arrayList2 :\n");
    printArrayList(&arrayList2);

    arraylist_insert(&arrayList1, 2, 4);
    arraylist_append(&arrayList1, 6);
    printf("\nArrayList1 before sorted :");
    printArrayList(&arrayList1);
    arraylist_sort(&arrayList1, &SortDownFunc);
    printf("\nArrayList1 after sorted SortDown:");
    printArrayList(&arrayList1);
    arraylist_sort(&arrayList1, &SortUpFunc);
    printf("\nArrayList1 after sorted SortUp:");
    printArrayList(&arrayList1);
}

/* Automatically resizing array */

ArrayList *arraylist_new(unsigned int length)
{
    /* If the length is not specified, use a sensible default */

    if (length <= 0) {
        length = 16;
    }

    /* Allocate the new ArrayList and fill in the fields.  There are
     * initially no entries. */

    ArrayList *newArraylist = (ArrayList *) malloc(sizeof(ArrayList));

    if (newArraylist == NULL) {
        return NULL;
    }

    newArraylist->_alloced = length;
    newArraylist->length = 0;

    /* Allocate the data array */

    newArraylist->data = malloc(length * sizeof(ArrayListValue));

    if (newArraylist->data == NULL) {
        free(newArraylist);
        return NULL;
    }

    return newArraylist;
}

void arraylist_free(ArrayList *arraylist)
{
    /* Do not free if a NULL pointer is passed */

    if (arraylist != NULL) {
        free(arraylist->data);
        free(arraylist);
    }


}


static int arraylist_enlarge(ArrayList *arraylist)
{
    /* Double the allocated size */

    unsigned int newSize = arraylist->_alloced * 2;

    /* Reallocate the array to the new size */

    ArrayListValue *data = realloc(arraylist->data, sizeof(ArrayListValue) * newSize);

    if (data == NULL) {
        return 0;
    } else {
        arraylist->data = data;
        arraylist->_alloced = newSize;

        return 1;
    }
}

int arraylist_insert(ArrayList *arraylist,
                     unsigned int index,
                     ArrayListValue data)
{
    /* Sanity check the index */

    if (index > arraylist->length) {
        return 0;
    }

    /* Increase the size if necessary */

    if (arraylist->length + 1 > arraylist->_alloced) {
        if (!arraylist_enlarge(arraylist)) {
            return 0;
        }
    }

    /* Move the contents of the array forward from the index
     * onwards */

    memmove(&arraylist->data[index + 1],
            &arraylist->data[index],
            (arraylist->length - index) * sizeof(ArrayListValue));

    /* Insert the new entry at the index */

    arraylist->data[index] = data;
    ++arraylist->length;

    return 1;
}

int arraylist_append(ArrayList *arraylist, ArrayListValue data)
{
    return arraylist_insert(arraylist, arraylist->length, data);
}

int arraylist_prepend(ArrayList *arraylist, ArrayListValue data)
{
    return arraylist_insert(arraylist, 0, data);
}

void arraylist_removeRange(ArrayList *arraylist,
                           unsigned int index,
                           unsigned int length)
{
    /* Check this is a valid range */

    if (index > arraylist->length || index + length > arraylist->length) {
        return;
    }

    /* Move back the entries following the range to be removed */

    memmove(&arraylist->data[index],
            &arraylist->data[index + length],
            (arraylist->length - (index + length))
                * sizeof(ArrayListValue));

    /* Decrease the counter */

    arraylist->length -= length;
}

void arraylist_remove(ArrayList *arraylist, unsigned int index)
{
    arraylist_removeRange(arraylist, index, 1);
}

int arraylist_indexOf(ArrayList *arraylist,
                      ArrayListEqualFunc callback,
                      ArrayListValue data)
{
    unsigned int i;

    for (i=0; i<arraylist->length; ++i) {
        if (callback(arraylist->data[i], data) != 0)
            return (int) i;
    }

    return -1;
}

void arraylist_clear(ArrayList *arraylist)
{
    /* To clear the list, simply set the length to zero */

    arraylist->length = 0;
}

static void arraylist_sortInternal(ArrayListValue *listData,
                                   unsigned int listLength,
                                   ArrayListCompareFunc compareFunc)
{
    /* If less than two items, it is always sorted. */

    if (listLength <= 1) {
        return;
    }

    /* Take the last item as the pivot. */

    ArrayListValue pivot = listData[listLength-1];

    /* Divide the list into two lists:
     *
     * List 1 contains data less than the pivot.
     * List 2 contains data more than the pivot.
     *
     * As the lists are build up, they are stored sequentially after
     * each other, ie. list_data[list1_length-1] is the last item
     * in list 1, list_data[list1_length] is the first item in
     * list 2.
     */

    ArrayListValue tmp;
    unsigned int i;
    unsigned int list1Length = 0;

    for (i=0; i<listLength-1; ++i) {

        if (compareFunc(listData[i], pivot) < 0) {

            /* This should be in list 1.  Therefore it is in the
             * wrong position. Swap the data immediately following
             * the last item in list 1 with this data. */

            tmp = listData[i];
            listData[i] = listData[list1Length];
            listData[list1Length] = tmp;

            ++list1Length;

        } else {
            /* This should be in list 2.  This is already in the
             * right position. */
        }
    }

    /* The length of list 2 can be calculated. */

    unsigned int list2Length = listLength - list1Length - 1;

    /* list_data[0..list1_length-1] now contains all items which are
     * before the pivot.
     * list_data[list1_length..list_length-2] contains all items after
     * or equal to the pivot. */

    /* Move the pivot into place, by swapping it with the item
     * immediately following the end of list 1.  */

    listData[listLength-1] = listData[list1Length];
    listData[list1Length] = pivot;

    /* Recursively sort the sublists. */

    arraylist_sortInternal(listData, list1Length, compareFunc);

    arraylist_sortInternal(&listData[list1Length + 1], list2Length,
                            compareFunc);
}

void arraylist_sort(ArrayList *arraylist, ArrayListCompareFunc compareFunc)
{
    /* Perform the recursive sort */

    arraylist_sortInternal(arraylist->data, arraylist->length,
                            compareFunc);
}


