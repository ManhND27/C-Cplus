#include <stdlib.h>

#include "dsbinaryheap.h"


struct _BinaryHeap {
    BinaryHeapType heapType;
	BinaryHeapValue *values;
    unsigned int numValues;
    unsigned int allocedSize;
    BinaryHeapCompareFunc compareFunc;
};

static int binaryheap_compare(BinaryHeap *heap,
                              BinaryHeapValue data1,
                              BinaryHeapValue data2)
{
    if (heap->heapType == BINARY_HEAP_TYPE_MIN) {
        return heap->compareFunc(data1, data2);
	} else {
        return -heap->compareFunc(data1, data2);
	}
}

BinaryHeap *binaryheap_new(BinaryHeapType heapType,
                           BinaryHeapCompareFunc compareFunc)
{
    BinaryHeap *heap = malloc(sizeof(BinaryHeap));

	if (heap == NULL) {
		return NULL;
	}

    heap->heapType = heapType;
    heap->numValues = 0;
    heap->compareFunc = compareFunc;

	/* Initial size of 16 elements */

    heap->allocedSize = 16;
    heap->values = malloc(sizeof(BinaryHeapValue) * heap->allocedSize);

	if (heap->values == NULL) {
		free(heap);
		return NULL;
	}

	return heap;
}

void binaryheap_free(BinaryHeap *heap)
{
	free(heap->values);
	free(heap);
}

int binaryheap_insert(BinaryHeap *heap, BinaryHeapValue value)
{
	/* Possibly realloc the heap to a larger size */

    BinaryHeapValue *newValues;
    unsigned int newSize;

    if (heap->numValues >= heap->allocedSize) {

		/* Double the table size */

        newSize = heap->allocedSize * 2;
        newValues = realloc(heap->values,
                            sizeof(BinaryHeapValue) * newSize);

        if (newValues == NULL) {
			return 0;
		}

        heap->allocedSize = newSize;
        heap->values = newValues;
	}

	/* Add to the bottom of the heap and start from there */

    unsigned int index = heap->numValues;
    ++heap->numValues;

	/* Percolate the value up to the top of the heap */

    unsigned int parent;

	while (index > 0) {

		/* The parent index is found by halving the node index */

		parent = (index - 1) / 2;

		/* Compare the node with its parent */

        if (binaryheap_compare(heap, heap->values[parent], value) < 0) {

			/* Ordered correctly - insertion is complete */

			break;

		} else {

			/* Need to swap this node with its parent */

			heap->values[index] = heap->values[parent];

			/* Advance up to the parent */

			index = parent;
		}
	}

	/* Save the new value in the final location */

	heap->values[index] = value;

	return 1;
}

BinaryHeapValue binaryheap_pop(BinaryHeap *heap)
{
	/* Empty heap? */

    if (heap->numValues == 0) {
		return BINARY_HEAP_NULL;
	}

	/* Take the value from the top of the heap */

    BinaryHeapValue result = heap->values[0];

	/* Remove the last value from the heap; we will percolate this down
	 * from the top. */

    BinaryHeapValue newValue = heap->values[heap->numValues - 1];
    --heap->numValues;

	/* Percolate the new top value down */

    unsigned int index = 0;
    unsigned int nextIndex;
    unsigned int child1;
    unsigned int child2;

	for (;;) {

		/* Calculate the array indexes of the children of this node */

		child1 = index * 2 + 1;
		child2 = index * 2 + 2;

        if (child1 < heap->numValues
                && binaryheap_compare(heap,
                                      newValue,
                                      heap->values[child1]) > 0) {

			/* Left child is less than the node.  We need to swap
			 * with one of the children, whichever is less. */

            if (child2 < heap->numValues
                    && binaryheap_compare(heap,
                                          heap->values[child1],
                                          heap->values[child2]) > 0) {
                nextIndex = child2;
			} else {
                nextIndex = child1;
			}

        } else if (child2 < heap->numValues
                   && binaryheap_compare(heap,
                                         newValue,
                                         heap->values[child2]) > 0) {

			/* Right child is less than the node.  Swap with the
			 * right child. */

            nextIndex = child2;

		} else {
			/* Node is less than both its children. The heap
			 * condition is satisfied.  * We can stop percolating
			 * down. */

            heap->values[index] = newValue;
			break;
		}

		/* Swap the current node with the least of the child nodes. */

        heap->values[index] = heap->values[nextIndex];

		/* Advance to the child we chose */

        index = nextIndex;
	}

	return result;
}

unsigned int binaryheap_numEntries(BinaryHeap *heap)
{
    return heap->numValues;
}
