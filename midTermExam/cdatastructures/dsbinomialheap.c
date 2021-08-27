#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "dsbinomialheap.h"


typedef struct _BinomialTree BinomialTree;

struct _BinomialTree
{
	BinomialHeapValue value;
	unsigned short order;
    unsigned short refCount;
	BinomialTree **subtrees;
};

struct _BinomialHeap
{
    BinomialHeapType heapType;
    BinomialHeapCompareFunc compareFunc;
    unsigned int numValues;
	BinomialTree **roots;
    unsigned int rootsLength;
};

static int binomialheap_compare(BinomialHeap *heap,
                                BinomialHeapValue data1,
                                BinomialHeapValue data2)
{
    if (heap->heapType == BINOMIAL_HEAP_TYPE_MIN) {
        return heap->compareFunc(data1, data2);
	} else {
        return -(heap->compareFunc(data1, data2));
	}
}

static void binomialtree_ref(BinomialTree *tree)
{
	if (tree != NULL) {
        ++tree->refCount;
	}
}

static void binomialtree_unref(BinomialTree *tree)
{
	if (tree == NULL) {
		return;
	}

	/* Subtract a reference */

    --tree->refCount;

	/* If this removed the last reference, unreference all subtrees
	 * and free. */

    if (tree->refCount == 0) {
        int i;
		for (i=0; i<tree->order; ++i) {
            binomialtree_unref(tree->subtrees[i]);
		}

		free(tree->subtrees);
		free(tree);
	}
}

static BinomialTree *binomialtree_merge(BinomialHeap *heap,
                                        BinomialTree *tree1,
                                        BinomialTree *tree2)
{
	/* Order tree1 and tree2 so that tree1 is the tree with the
	 * smallest root */

    BinomialTree *tmp;

    if (binomialheap_compare(heap, tree1->value, tree2->value) > 0) {

		/* Swap tree1 and tree2 */

		tmp = tree1;
		tree1 = tree2;
		tree2 = tmp;
	}

	/* Allocate a new tree */

    BinomialTree *newTree = malloc(sizeof(BinomialTree));

    if (newTree == NULL) {
		return NULL;
	}

    newTree->refCount = 0;
    newTree->order = (unsigned short) (tree1->order + 1);

	/* Take the smallest value of the two trees */

    newTree->value = tree1->value;

	/* Copy subtrees of the smallest tree.  The last entry in the
	 * array is the larger tree */

    newTree->subtrees = malloc(sizeof(BinomialTree *) * newTree->order);

    if (newTree->subtrees == NULL) {
        free(newTree);
		return NULL;
	}

    memcpy(newTree->subtrees, tree1->subtrees,
	       sizeof(BinomialTree *) * tree1->order);
    newTree->subtrees[newTree->order - 1] = tree2;

	/* Add a reference to each of the subtrees we have referenced */

    int i;
    for (i=0; i<newTree->order; ++i) {
        binomialtree_ref(newTree->subtrees[i]);
	}

    return newTree;
}

/* Used to perform an "undo" when an error occurs during
 * binomial_heap_merge.  Go through the list of roots so far and remove
 * references that have been added. */

static void binomialheap_mergeUndo(BinomialTree **newRoots, unsigned int count)
{
	unsigned int i;

	for (i=0; i<=count; ++i) {
        binomialtree_unref(newRoots[i]);
	}

    free(newRoots);
}

/* Merge the data in the 'other' heap into the 'heap' heap.
 * Returns non-zero if successful. */

static int binomialheap_merge(BinomialHeap *heap, BinomialHeap *other)
{
	/* Find the maximum length of the two heaps.  Add one because
	 * after merging we may have one more value to carry over. */

    unsigned int max;

    if (heap->rootsLength > other->rootsLength) {
        max = heap->rootsLength + 1;
	} else {
        max = other->rootsLength + 1;
	}

	/* Allocate an array for the new roots */

    BinomialTree **newRoots = malloc(sizeof(BinomialTree *) * max);

    if (newRoots == NULL) {
		return 0;
	}

	/* Go through one entry at a time.  This works kind of like a
	 * ripple-carry adder. */

    BinomialTree *vals[3];
    int numVals;
    unsigned int newRootsLength = 0;
    BinomialTree *carry = NULL;
    BinomialTree *newCarry;
    unsigned int i;

	for (i=0; i<max; ++i) {

		/* Build up 'vals' as a list of all the values we must
		 * merge at this step. */

        numVals = 0;

		/* If there is a value in 'heap', add it */

        if (i < heap->rootsLength && heap->roots[i] != NULL) {
            vals[numVals] = heap->roots[i];
            ++numVals;
		}

		/* If there is a value in 'other', add it */

        if (i < other->rootsLength && other->roots[i] != NULL) {
            vals[numVals] = other->roots[i];
            ++numVals;
		}

		/* If there is a carried value from the previous iteration,
		 * add it */

		if (carry != NULL) {
            vals[numVals] = carry;
            ++numVals;
		}

		/* When num_vals == 1 or 3, we store a value. */

        if ((numVals & 1) != 0) {

			/* Save the last value into new_roots. */

            newRoots[i] = vals[numVals - 1];
            binomialtree_ref(newRoots[i]);
            newRootsLength = i + 1;

		} else {

			/* No value to store at this iteration */

            newRoots[i] = NULL;
		}

		/* When num_vals == 2 or 3, we must carry over to the
		 * next iteration */

        if ((numVals & 2) != 0) {

			/* Merge the first two values and carry to the
			 * next iteration */

            newCarry = binomialtree_merge(heap, vals[0], vals[1]);

            if (newCarry == NULL) {

				/* Remove references that we have added
				 * (freeing any BinomialTree structures
				 * that were created in the process) */

                binomialheap_mergeUndo(newRoots, i);

				/* Unreference the carry variable */

                binomialtree_unref(carry);

				return 0;
			}

		} else {

			/* Nothing to carry */

            newCarry = NULL;
		}

		/* Unreference previous carried value */

        binomialtree_unref(carry);

		/* Assign the new value of carry, and add a reference */

        carry = newCarry;

        binomialtree_ref(carry);
	}

	/* Unreference all values in the old 'roots' array, freeing unused
	 * BinomialTree structures as necessary. */

    for (i=0; i<heap->rootsLength; ++i) {
		if (heap->roots[i] != NULL) {
            binomialtree_unref(heap->roots[i]);
		}
	}

	/* Free the old roots array and use the new one */

	free(heap->roots);
    heap->roots = newRoots;
    heap->rootsLength = newRootsLength;

	/* Merged successfully */

	return 1;
}

BinomialHeap *binomialheap_new(BinomialHeapType heapType,
                               BinomialHeapCompareFunc compareFunc)
{
	/* Allocate a new heap */

    BinomialHeap *newHeap = calloc(1, sizeof(BinomialHeap));

    if (newHeap == NULL) {
		return NULL;
	}

	/* Initialise and return */

    newHeap->heapType = heapType;
    newHeap->compareFunc = compareFunc;

    return newHeap;
}

void binomialheap_free(BinomialHeap *heap)
{
	/* Unreference all trees in the heap.  This should free
	 * back all subtrees. */

    unsigned int i;
    for (i=0; i<heap->rootsLength; ++i) {
        binomialtree_unref(heap->roots[i]);
	}

	/* Free the heap itself */

	free(heap->roots);
	free(heap);
}

int binomialheap_insert(BinomialHeap *heap, BinomialHeapValue value)
{
	/* Allocate an order 0 tree for storing the new value */

    BinomialTree *newTree = malloc(sizeof(BinomialTree));

    if (newTree == NULL) {
		return 0;
	}

	/* Fill in values.  This has an initial reference count of 1 that
	 * the "fake" heap holds; this will be removed at the end of
	 * this function. */

    newTree->value = value;
    newTree->order = 0;
    newTree->refCount = 1;
    newTree->subtrees = NULL;

	/* Build a fake heap structure for merging */

    BinomialHeap fakeHeap;
    fakeHeap.heapType = heap->heapType;
    fakeHeap.compareFunc = heap->compareFunc;
    fakeHeap.numValues = 1;
    fakeHeap.roots = &newTree;
    fakeHeap.rootsLength = 1;

	/* Perform the merge */

    int result = binomialheap_merge(heap, &fakeHeap);

	if (result != 0) {
        ++heap->numValues;
	}

	/* Remove reference to the new tree. */

    binomialtree_unref(newTree);

	return result;
}

BinomialHeapValue binomialheap_pop(BinomialHeap *heap)
{
    if (heap->numValues == 0) {
		return BINOMIAL_HEAP_NULL;
	}

	/* Find the tree with the lowest root value */

    unsigned int leastIndex = UINT_MAX;
    unsigned int i;

    for (i=0; i<heap->rootsLength; ++i) {

		if (heap->roots[i] == NULL) {
			continue;
		}

        if (leastIndex == UINT_MAX
                || binomialheap_compare(heap,
                                        heap->roots[i]->value,
                                        heap->roots[leastIndex]->value) < 0) {
            leastIndex = i;
		}
	}

	/* Remove the least_tree from the heap. */

    BinomialTree *leastTree = heap->roots[leastIndex];
    heap->roots[leastIndex] = NULL;

	/* Construct a fake heap containing the data in the least tree */

    BinomialHeap fakeHeap;
    fakeHeap.heapType = heap->heapType;
    fakeHeap.compareFunc = heap->compareFunc;
    fakeHeap.roots = leastTree->subtrees;
    fakeHeap.rootsLength = leastTree->order;

	/* Merge subtrees of least tree back into the heap */

    BinomialHeapValue result;
    if (binomialheap_merge(heap, &fakeHeap)) {

		/* Merge successful */

		/* Remove reference to least tree */

        result = leastTree->value;
        binomialtree_unref(leastTree);

		/* Update the number of values */

        --heap->numValues;

		return result;

	} else {

		/* Add the least tree back */

        heap->roots[leastIndex] = leastTree;

		/* Pop failed */

		return BINOMIAL_HEAP_NULL;
	}
}

unsigned int binomialheap_numEntries(BinomialHeap *heap)
{
    return heap->numValues;
}

