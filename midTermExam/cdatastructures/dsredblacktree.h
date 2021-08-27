/** @file dsredblacktree.h
 *
 * @brief Balanced binary tree
 *
 * The red-black tree structure is a balanced binary tree which stores
 * a collection of nodes (see @ref RBTreeNode).  Each node has
 * a key and a value associated with it.  The nodes are sorted
 * within the tree based on the order of their keys. Modifications
 * to the tree are constructed such that the tree remains
 * balanced at all times (there are always roughly equal numbers
 * of nodes on either side of the tree).
 *
 * Balanced binary trees have several uses.  They can be used
 * as a mapping (searching for a value based on its key), or
 * as a set of keys which is always ordered.
 *
 * To create a new red-black tree, use @ref rbtree_new.  To destroy
 * a red-black tree, use @ref rbtree_free.
 *
 * To insert a new key-value pair into a red-black tree, use
 * @ref rbtree_insert.  To remove an entry from a
 * red-black tree, use @ref rbtree_remove or @ref rbtree_removeNode.
 *
 * To search a red-black tree, use @ref rbtree_lookup or
 * @ref rbtree_lookupNode.
 *
 * Tree nodes can be queried using the
 * @ref rbtree_nodeLeftChild,
 * @ref rbtree_nodeRightChild,
 * @ref rbtree_nodeParent,
 * @ref rbtree_nodeKey and
 * @ref rbtree_nodeValue functions.
 */

#ifndef DSREDBLACKTREE_H
#define DSREDBLACKTREE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A red-black tree balanced binary tree.
 *
 * @see rbtree_new
 */

typedef struct _RBTree RBTree;

/**
 * A key for an @ref RBTree.
 */

typedef void *RBTreeKey;

/**
 * A value stored in an @ref RBTree.
 */

typedef void *RBTreeValue;

/**
 * A null @ref RBTreeValue.
 */

#define RB_TREE_NULL ((void *) 0)

/**
 * A node in a red-black tree.
 *
 * @see rbtree_nodeLeftChild
 * @see rbtree_nodeRightChild
 * @see rbtree_nodeParent
 * @see rbtree_nodeKey
 * @see rbtree_nodeValue
 */

typedef struct _RBTreeNode RBTreeNode;

/**
 * Type of function used to compare keys in a red-black tree.
 *
 * @param data1            The first key.
 * @param data2            The second key.
 * @return                 A negative number if data1 should be sorted
 *                         before data2, a positive number if data2 should
 *                         be sorted before data1, zero if the two keys
 *                         are equal.
 */

typedef int (*RBTreeCompareFunc)(RBTreeValue data1, RBTreeValue data2);

/**
 * Each node in a red-black tree is either red or black.
 */

typedef enum {
	RB_TREE_NODE_RED,
	RB_TREE_NODE_BLACK,
} RBTreeNodeColor;

/**
 * A @ref RBTreeNode can have left and right children.
 */

typedef enum {
	RB_TREE_NODE_LEFT = 0,
	RB_TREE_NODE_RIGHT = 1
} RBTreeNodeSide;

/**
 * Create a new red-black tree.
 *
 * @param compareFunc     Function to use when comparing keys in the tree.
 * @return                A new red-black tree, or NULL if it was not possible
 *                        to allocate the memory.
 */

RBTree *rbtree_new(RBTreeCompareFunc compareFunc);

/**
 * Destroy a red-black tree.
 *
 * @param tree            The tree to destroy.
 */

void rbtree_free(RBTree *tree);

/**
 * Insert a new key-value pair into a red-black tree.
 *
 * @param tree            The tree.
 * @param key             The key to insert.
 * @param value           The value to insert.
 * @return                The newly created tree node containing the
 *                        key and value, or NULL if it was not possible
 *                        to allocate the new memory.
 */

RBTreeNode *rbtree_insert(RBTree *tree, RBTreeKey key, RBTreeValue value);

/**
 * Remove a node from a tree.
 *
 * @param tree            The tree.
 * @param node            The node to remove
 */

void rbtree_removeNode(RBTree *tree, RBTreeNode *node);

/**
 * Remove an entry from a tree, specifying the key of the node to
 * remove.
 *
 * @param tree            The tree.
 * @param key             The key of the node to remove.
 * @return                Zero (false) if no node with the specified key was
 *                        found in the tree, non-zero (true) if a node with
 *                        the specified key was removed.
 */

int rbtree_remove(RBTree *tree, RBTreeKey key);

/**
 * Search a red-black tree for a node with a particular key.  This uses
 * the tree as a mapping.
 *
 * @param tree            The red-black tree to search.
 * @param key             The key to search for.
 * @return                The tree node containing the given key, or NULL
 *                        if no entry with the given key is found.
 */

RBTreeNode *rbtree_lookupNode(RBTree *tree, RBTreeKey key);

/**
 * Search a red-black tree for a value corresponding to a particular key.
 * This uses the tree as a mapping.  Note that this performs
 * identically to @ref rb_tree_lookup_node, except that the value
 * at the node is returned rather than the node itself.
 *
 * @param tree            The red-black tree to search.
 * @param key             The key to search for.
 * @return                The value associated with the given key, or
 *                        RB_TREE_NULL if no entry with the given key is
 *                        found.
 */

RBTreeValue rbtree_lookup(RBTree *tree, RBTreeKey key);

/**
 * Find the root node of a tree.
 *
 * @param tree            The tree.
 * @return                The root node of the tree, or NULL if the tree is
 *                        empty.
 */

RBTreeNode *rbtree_rootNode(RBTree *tree);

/**
 * Retrieve the key for a given tree node.
 *
 * @param node            The tree node.
 * @return                The key to the given node.
 */

RBTreeKey rbtree_nodeKey(RBTreeNode *node);

/**
 * Retrieve the value at a given tree node.
 *
 * @param node            The tree node.
 * @return                The value at the given node.
 */

RBTreeValue rbtree_nodeValue(RBTreeNode *node);

/**
 * Get a child of a given tree node.
 *
 * @param node            The tree node.
 * @param side            The side relative to the node.
 * @return                The child of the tree node, or NULL if the
 *                        node has no child on the specified side.
 */

RBTreeNode *rbtree_nodeChild(RBTreeNode *node, RBTreeNodeSide side);

/**
 * Find the parent node of a given tree node.
 *
 * @param node            The tree node.
 * @return                The parent node of the tree node, or NULL if
 *                        this is the root node.
 */

RBTreeNode *rbtree_nodeParent(RBTreeNode *node);

/**
 * Find the height of a subtree.
 *
 * @param node            The root node of the subtree.
 * @return                The height of the subtree.
 */

int rbtree_subtreeHeight(RBTreeNode *node);

/**
 * Convert the keys in a red-black tree into a C array.  This allows
 * the tree to be used as an ordered set.
 *
 * @param tree            The tree.
 * @return                A newly allocated C array containing all the keys
 *                        in the tree, in order.  The length of the array
 *                        is equal to the number of entries in the tree
 *                        (see @ref rb_tree_num_entries).
 */

RBTreeValue *rbtree_toArray(RBTree *tree);

/**
 * Retrieve the number of entries in the tree.
 *
 * @param tree            The tree.
 * @return                The number of key-value pairs stored in the tree.
 */

int rbtree_numEntries(RBTree *tree);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSREDBLACKTREE_H */

