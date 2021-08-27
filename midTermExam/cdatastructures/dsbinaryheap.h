/**
 * @file dsbinaryheap.h
 *
 * @brief Binary heap.
 *
 * A binary heap is a heap data structure implemented using a
 * binary tree.  In a heap, values are ordered by priority.
 *
 * To create a binary heap, use @ref binaryheap_new.  To destroy a
 * binary heap, use @ref binaryheap_free.
 *
 * To insert a value into a binary heap, use @ref binaryheap_insert.
 *
 * To remove the first value from a binary heap, use @ref binaryheap_pop.
 *
 */

#ifndef DSBINARYHEAP_H
#define DSBINARYHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Heap type.  If a heap is a min heap (@ref BINARY_HEAP_TYPE_MIN), the
 * values with the lowest priority are stored at the top of the heap and
 * will be the first returned.  If a heap is a max heap
 * (@ref BINARY_HEAP_TYPE_MAX), the values with the greatest priority are
 * stored at the top of the heap.
 */

typedef enum {
	/** A minimum heap. */

	BINARY_HEAP_TYPE_MIN,

	/** A maximum heap. */

	BINARY_HEAP_TYPE_MAX
} BinaryHeapType;

/**
 * A value stored in a @ref BinaryHeap.
 */

typedef void *BinaryHeapValue;

/**
 * A null @ref BinaryHeapValue.
 */

#define BINARY_HEAP_NULL ((void *) 0)

/**
 * Type of function used to compare values in a binary heap.
 *
 * @param value1           The first value.
 * @param value2           The second value.
 * @return                 A negative number if value1 is less than value2,
 *                         a positive number if value1 is greater than value2,
 *                         zero if the two are equal.
 */

typedef int (*BinaryHeapCompareFunc)(BinaryHeapValue value1,
                                     BinaryHeapValue value2);

/**
 * A binary heap data structure.
 */

typedef struct _BinaryHeap BinaryHeap;

/**
 * Create a new @ref BinaryHeap.
 *
 * @param heapType         The type of heap: min heap or max heap.
 * @param compareFunc      Pointer to a function used to compare the priority
 *                         of values in the heap.
 * @return                 A new binary heap, or NULL if it was not possible
 *                         to allocate the memory.
 */

BinaryHeap *binaryheap_new(BinaryHeapType heapType,
                           BinaryHeapCompareFunc compareFunc);

/**
 * Destroy a binary heap.
 *
 * @param heap             The heap to destroy.
 */

void binaryheap_free(BinaryHeap *heap);

/**
 * Insert a value into a binary heap.
 *
 * @param heap             The heap to insert into.
 * @param value            The value to insert.
 * @return                 Non-zero if the entry was added, or zero if it
 *                         was not possible to allocate memory for the new
 *                         entry.
 */

int binaryheap_insert(BinaryHeap *heap, BinaryHeapValue value);

/**
 * Remove the first value from a binary heap.
 *
 * @param heap             The heap.
 * @return                 The first value in the heap, or
 *                         @ref BINARY_HEAP_NULL if the heap is empty.
 */

BinaryHeapValue binaryheap_pop(BinaryHeap *heap);

/**
 * Find the number of values stored in a binary heap.
 *
 * @param heap             The heap.
 * @return                 The number of values in the heap.
 */

unsigned int binaryheap_numEntries(BinaryHeap *heap);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSBINARYHEAP_H */

