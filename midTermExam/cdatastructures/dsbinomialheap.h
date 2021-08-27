/**
 * @file dsbinomialheap.h
 *
 * @brief Binomial heap.
 *
 * A binomial heap is a heap data structure implemented using a
 * binomial tree.  In a heap, values are ordered by priority.
 *
 * To create a binomial heap, use @ref binomial_heap_new.  To destroy a
 * binomial heap, use @ref binomialheap_free.
 *
 * To insert a value into a binomial heap, use @ref binomialheap_insert.
 *
 * To remove the first value from a binomial heap, use @ref binomialheap_pop.
 *
 */

#ifndef DSBINOMIALHEAP_H
#define DSBINOMIALHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Heap type.  If a heap is a min heap (@ref BINOMIAL_HEAP_TYPE_MIN), the
 * values with the lowest priority are stored at the top of the heap and
 * will be the first returned.  If a heap is a max heap
 * (@ref BINOMIAL_HEAP_TYPE_MAX), the values with the greatest priority
 * are stored at the top of the heap.
 */

typedef enum {
	/** A minimum heap. */

	BINOMIAL_HEAP_TYPE_MIN,

	/** A maximum heap. */

	BINOMIAL_HEAP_TYPE_MAX
} BinomialHeapType;

/**
 * A value stored in a @ref BinomialHeap.
 */

typedef void *BinomialHeapValue;

/**
 * A null @ref BinomialHeapValue.
 */

#define BINOMIAL_HEAP_NULL ((void *) 0)

/**
 * Type of function used to compare values in a binomial heap.
 *
 * @param value1           The first value.
 * @param value2           The second value.
 * @return                 A negative number if value1 is less than value2,
 *                         a positive number if value1 is greater than value2,
 *                         zero if the two are equal.
 */

typedef int (*BinomialHeapCompareFunc)(BinomialHeapValue value1,
                                       BinomialHeapValue value2);

/**
 * A binomial heap data structure.
 */

typedef struct _BinomialHeap BinomialHeap;

/**
 * Create a new @ref BinomialHeap.
 *
 * @param heapType         The type of heap: min heap or max heap.
 * @param compareFunc      Pointer to a function used to compare the priority
 *                         of values in the heap.
 * @return                 A new binomial heap, or NULL if it was not possible
 *                         to allocate the memory.
 */

BinomialHeap *binomialheap_new(BinomialHeapType heapType,
                               BinomialHeapCompareFunc compareFunc);

/**
 * Destroy a binomial heap.
 *
 * @param heap             The heap to destroy.
 */

void binomialheap_free(BinomialHeap *heap);

/**
 * Insert a value into a binomial heap.
 *
 * @param heap             The heap to insert into.
 * @param value            The value to insert.
 * @return                 Non-zero if the entry was added, or zero if it
 *                         was not possible to allocate memory for the new
 *                         entry.
 */

int binomialheap_insert(BinomialHeap *heap, BinomialHeapValue value);

/**
 * Remove the first value from a binomial heap.
 *
 * @param heap             The heap.
 * @return                 The first value in the heap, or
 *                         @ref BINOMIAL_HEAP_NULL if the heap is empty.
 */

BinomialHeapValue binomialheap_pop(BinomialHeap *heap);

/**
 * Find the number of values stored in a binomial heap.
 *
 * @param heap             The heap.
 * @return                 The number of values in the heap.
 */

unsigned int binomialheap_numEntries(BinomialHeap *heap);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSBINOMIALHEAP_H */

