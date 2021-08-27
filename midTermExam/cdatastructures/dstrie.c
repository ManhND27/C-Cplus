/* Trie: fast mapping of strings to values */

#include <stdlib.h>
#include <string.h>

#include "dstrie.h"


typedef struct _TrieNode TrieNode;

struct _TrieNode {
	TrieValue data;
    unsigned int useCount;
	TrieNode *next[256];
};

struct _Trie {
    TrieNode *rootNode;
};

Trie *trie_new(void)
{
    Trie *newTrie = (Trie *) malloc(sizeof(Trie));
    if (newTrie == NULL) {
		return NULL;
	}

    newTrie->rootNode = NULL;

    return newTrie;
}

static void trie_freeListPush(TrieNode **list, TrieNode *node)
{
	node->data = *list;
	*list = node;
}

static TrieNode *trie_freeListPop(TrieNode **list)
{
    TrieNode *result = *list;
	*list = result->data;

	return result;
}

void trie_free(Trie *trie)
{
    TrieNode *freeList = NULL;

	/* Start with the root node */

    if (trie->rootNode != NULL) {
        trie_freeListPush(&freeList, trie->rootNode);
	}

	/* Go through the free list, freeing nodes.  We add new nodes as
	 * we encounter them; in this way, all the nodes are freed
	 * non-recursively. */

    TrieNode *node;

    while (freeList != NULL) {
        node = trie_freeListPop(&freeList);

		/* Add all children of this node to the free list */

        int i;
		for (i=0; i<256; ++i) {
			if (node->next[i] != NULL) {
                trie_freeListPush(&freeList, node->next[i]);
			}
		}

		/* Free the node */

		free(node);
	}

	/* Free the trie */

	free(trie);
}

static TrieNode *trie_findEnd(Trie *trie, char *key)
{
	/* Search down the trie until the end of string is reached */

    TrieNode *node = trie->rootNode;

    char *p;

	for (p=key; *p != '\0'; ++p) {

		if (node == NULL) {
			/* Not found in the tree. Return. */

			return NULL;
		}

		/* Jump to the next node */

		node = node->next[(unsigned char) *p];
	}

	/* This key is present if the value at the last node is not NULL */

	return node;
}

static TrieNode *trie_findEndBinary(Trie *trie,
                                    unsigned char *key,
                                    int keyLength)
{
	/* Search down the trie until the end of string is reached */

    TrieNode *node = trie->rootNode;
    int j;
    int c;

    for (j=0; j<keyLength; j++) {

		if (node == NULL) {
			/* Not found in the tree. Return. */
			return NULL;
		}

		c = (unsigned char) key[j];

		/* Jump to the next node */

		node = node->next[c];
	}

	/* This key is present if the value at the last node is not NULL */

	return node;
}

/* Roll back an insert operation after a failed malloc() call. */

static void trie_insertRollback(Trie *trie, unsigned char *key)
{
	/* Follow the chain along.  We know that we will never reach the
	 * end of the string because trie_insert never got that far.  As a
	 * result, it is not necessary to check for the end of string
	 * delimiter (NUL) */

    TrieNode *node = trie->rootNode;
    TrieNode **prevPtr = &trie->rootNode;
    unsigned char *p = key;
    TrieNode *nextNode;
    TrieNode **nextPrevPtr;

	while (node != NULL) {

		/* Find the next node now. We might free this node. */

        nextPrevPtr = &node->next[(unsigned char) *p];
        nextNode = *nextPrevPtr;
		++p;

		/* Decrease the use count and free the node if it
		 * reaches zero. */

        --node->useCount;

        if (node->useCount == 0) {
			free(node);

            if (prevPtr != NULL) {
                *prevPtr = NULL;
			}

            nextPrevPtr = NULL;
		}

		/* Update pointers */

        node = nextNode;
        prevPtr = nextPrevPtr;
	}
}

int trie_insert(Trie *trie, char *key, TrieValue value)
{
	/* Cannot insert NULL values */

	if (value == TRIE_NULL) {
		return 0;
	}

	/* Search to see if this is already in the tree */

    TrieNode *node = trie_findEnd(trie, key);

	/* Already in the tree? If so, replace the existing value and
	 * return success. */

	if (node != NULL && node->data != TRIE_NULL) {
		node->data = value;
		return 1;
	}

	/* Search down the trie until we reach the end of string,
	 * creating nodes as necessary */

    TrieNode **rover = &trie->rootNode;
    char *p = key;

	for (;;) {

		node = *rover;

		if (node == NULL) {

			/* Node does not exist, so create it */

			node = (TrieNode *) calloc(1, sizeof(TrieNode));

			if (node == NULL) {

				/* Allocation failed.  Go back and undo
				 * what we have done so far. */

                trie_insertRollback(trie,
				                     (unsigned char *) key);

				return 0;
			}

			node->data = TRIE_NULL;

			/* Link in to the trie */

			*rover = node;
		}

		/* Increase the node use count */

        ++node->useCount;

		/* Current character */

        int c = (unsigned char) *p;

		/* Reached the end of string?  If so, we're finished. */

		if (c == '\0') {

			/* Set the data at the node we have reached */

			node->data = value;

			break;
		}

		/* Advance to the next node in the chain */

		rover = &node->next[c];
		++p;
	}

	return 1;
}


int trie_insertBinary(Trie *trie,
                      unsigned char *key,
                      int keyLength,
                      TrieValue value)
{
	/* Cannot insert NULL values */

	if (value == TRIE_NULL) {
		return 0;
	}

	/* Search to see if this is already in the tree */

    TrieNode *node = trie_findEndBinary(trie, key, keyLength);

	/* Already in the tree? If so, replace the existing value and
	 * return success. */

	if (node != NULL && node->data != TRIE_NULL) {
		node->data = value;
		return 1;
	}

	/* Search down the trie until we reach the end of string,
	 * creating nodes as necessary */

    TrieNode **rover = &trie->rootNode;

    int p = 0;

	for (;;) {

		node = *rover;

		if (node == NULL) {

			/* Node does not exist, so create it */

			node = (TrieNode *) calloc(1, sizeof(TrieNode));

			if (node == NULL) {

				/* Allocation failed.  Go back and undo
				 * what we have done so far. */

                trie_insertRollback(trie, key);

				return 0;
			}

			node->data = TRIE_NULL;

			/* Link in to the trie */

			*rover = node;
		}

		/* Increase the node use count */

        ++node->useCount;

		/* Current character */

        int c = (unsigned char) key[p];

		/* Reached the end of string?  If so, we're finished. */

        if (p == keyLength) {

			/* Set the data at the node we have reached */

			node->data = value;

			break;
		}

		/* Advance to the next node in the chain */

		rover = &node->next[c];
		++p;
	}

	return 1;
}

int trie_removeBinary(Trie *trie, unsigned char *key, int keyLength)
{
	/* Find the end node and remove the value */

    TrieNode *node = trie_findEndBinary(trie, key, keyLength);

	if (node != NULL && node->data != TRIE_NULL) {
		node->data = TRIE_NULL;
	} else {
		return 0;
	}

	/* Now traverse the tree again as before, decrementing the use
	 * count of each node.  Free back nodes as necessary. */

    node = trie->rootNode;
    TrieNode **lastNextPtr = &trie->rootNode;
    int p = 0;
    int c;

    TrieNode *next;

	for (;;) {

		/* Find the next node */
		c = (unsigned char) key[p];
		next = node->next[c];

		/* Free this node if necessary */

        --node->useCount;

        if (node->useCount <= 0) {
			free(node);

			/* Set the "next" pointer on the previous node to NULL,
			 * to unlink the freed node from the tree.  This only
			 * needs to be done once in a remove.  After the first
			 * unlink, all further nodes are also going to be
			 * free'd. */

            if (lastNextPtr != NULL) {
                *lastNextPtr = NULL;
                lastNextPtr = NULL;
			}
		}

		/* Go to the next character or finish */
        if (p == keyLength) {
			break;
		} else {
			++p;
		}

		/* If necessary, save the location of the "next" pointer
		 * so that it may be set to NULL on the next iteration if
		 * the next node visited is freed. */

        if (lastNextPtr != NULL) {
            lastNextPtr = &node->next[c];
		}

		/* Jump to the next node */

		node = next;
	}

	/* Removed successfully */

	return 1;
}

int trie_remove(Trie *trie, char *key)
{
	/* Find the end node and remove the value */

    TrieNode *node = trie_findEnd(trie, key);

	if (node != NULL && node->data != TRIE_NULL) {
		node->data = TRIE_NULL;
	} else {
		return 0;
	}

	/* Now traverse the tree again as before, decrementing the use
	 * count of each node.  Free back nodes as necessary. */

    node = trie->rootNode;
    TrieNode **lastNextPtr = &trie->rootNode;
    char *p = key;
    int c;

    TrieNode *next;

	for (;;) {

		/* Find the next node */

		c = (unsigned char) *p;
		next = node->next[c];

		/* Free this node if necessary */

        --node->useCount;

        if (node->useCount <= 0) {
			free(node);

			/* Set the "next" pointer on the previous node to NULL,
			 * to unlink the freed node from the tree.  This only
			 * needs to be done once in a remove.  After the first
			 * unlink, all further nodes are also going to be
			 * free'd. */

            if (lastNextPtr != NULL) {
                *lastNextPtr = NULL;
                lastNextPtr = NULL;
			}
		}

		/* Go to the next character or finish */

		if (c == '\0') {
			break;
		} else {
			++p;
		}

		/* If necessary, save the location of the "next" pointer
		 * so that it may be set to NULL on the next iteration if
		 * the next node visited is freed. */

        if (lastNextPtr != NULL) {
            lastNextPtr = &node->next[c];
		}

		/* Jump to the next node */

		node = next;
	}

	/* Removed successfully */

	return 1;
}

TrieValue trie_lookup(Trie *trie, char *key)
{
    TrieNode *node = trie_findEnd(trie, key);

	if (node != NULL) {
		return node->data;
	} else {
		return TRIE_NULL;
	}
}

TrieValue trie_lookupBinary(Trie *trie, unsigned char *key, int keyLength)
{
    TrieNode *node = trie_findEndBinary(trie, key, keyLength);

	if (node != NULL) {
		return node->data;
	} else {
		return TRIE_NULL;
	}
}

unsigned int trie_numEntries(Trie *trie)
{
	/* To find the number of entries, simply look at the use count
	 * of the root node. */

    if (trie->rootNode == NULL) {
		return 0;
	} else {
        return trie->rootNode->useCount;
	}
}

