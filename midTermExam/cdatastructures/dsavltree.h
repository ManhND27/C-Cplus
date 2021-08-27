/** @file dsavltree.h
 *
 * @brief Balanced binary tree
 *
 * The AVL tree structure is a balanced binary tree which stores
 * a collection of nodes (see @ref AVLTreeNode).  Each node has
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
 * To create a new AVL tree, use @ref avltree_new.  To destroy
 * an AVL tree, use @ref avltree_free.
 *
 * To insert a new key-value pair into an AVL tree, use
 * @ref avltree_insert.  To remove an entry from an
 * AVL tree, use @ref avltree_remove or @ref avltree_removeNode.
 *
 * To search an AVL tree, use @ref avltree_lookup or
 * @ref avltree_lookupNode.
 *
 * Tree nodes can be queried using the
 * @ref avltree_nodeChild,
 * @ref avltree_nodeParent,
 * @ref avltree_nodeKey and
 * @ref avltree_nodeValue functions.
 */

#ifndef DSAVLTREE_H
#define DSAVLTREE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * An AVL tree balanced binary tree.
 *
 * @see avltree_new
 */

typedef struct _AVLTree AVLTree;

/**
 * A key for an @ref AVLTree.
 */

typedef void *AVLTreeKey;

/**
 * A value stored in an @ref AVLTree.
 */

typedef void *AVLTreeValue;

/**
 * A null @ref AVLTreeValue.
 */

#define AVL_TREE_NULL ((void *) 0)

/**
 * A node in an AVL tree.
 *
 * @see avltree_nodeLeftChild
 * @see avltree_nodeRightChild
 * @see avltree_nodeParent
 * @see avltree_nodeKey
 * @see avltree_nodeValue
 */

typedef struct _AVLTreeNode AVLTreeNode;

/**
 * An @ref AVLTreeNode can have left and right children.
 */

typedef enum {
	AVL_TREE_NODE_LEFT = 0,
	AVL_TREE_NODE_RIGHT = 1
} AVLTreeNodeSide;

/**
 * Type of function used to compare keys in an AVL tree.
 *
 * @param value1           The first key.
 * @param value2           The second key.
 * @return                 A negative number if value1 should be sorted
 *                         before value2, a positive number if value2 should
 *                         be sorted before value1, zero if the two keys
 *                         are equal.
 */

typedef int (*AVLTreeCompareFunc)(AVLTreeValue value1, AVLTreeValue value2);

/**
 * Create a new AVL tree.
 *
 * @param compareFunc     Function to use when comparing keys in the tree.
 * @return                A new AVL tree, or NULL if it was not possible
 *                        to allocate the memory.
 */

AVLTree *avltree_new(AVLTreeCompareFunc compareFunc);

/**
 * Destroy an AVL tree.
 *
 * @param tree            The tree to destroy.
 */

void avltree_free(AVLTree *tree);

/**
 * Insert a new key-value pair into an AVL tree.
 *
 * @param tree            The tree.
 * @param key             The key to insert.
 * @param value           The value to insert.
 * @return                The newly created tree node containing the
 *                        key and value, or NULL if it was not possible
 *                        to allocate the new memory.
 */

AVLTreeNode *avltree_insert(AVLTree *tree,
                            AVLTreeKey key,
                            AVLTreeValue value);

/**
 * Remove a node from a tree.
 *
 * @param tree            The tree.
 * @param node            The node to remove
 */

void avltree_removeNode(AVLTree *tree, AVLTreeNode *node);

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

int avltree_remove(AVLTree *tree, AVLTreeKey key);

/**
 * Search an AVL tree for a node with a particular key.  This uses
 * the tree as a mapping.
 *
 * @param tree            The AVL tree to search.
 * @param key             The key to search for.
 * @return                The tree node containing the given key, or NULL
 *                        if no entry with the given key is found.
 */

AVLTreeNode *avltree_lookupNode(AVLTree *tree, AVLTreeKey key);

/**
 * Search an AVL tree for a value corresponding to a particular key.
 * This uses the tree as a mapping.  Note that this performs
 * identically to @ref avl_tree_lookup_node, except that the value
 * at the node is returned rather than the node itself.
 *
 * @param tree            The AVL tree to search.
 * @param key             The key to search for.
 * @return                The value associated with the given key, or
 *                        @ref AVL_TREE_NULL if no entry with the given key is
 *                        found.
 */

AVLTreeValue avltree_lookup(AVLTree *tree, AVLTreeKey key);

/**
 * Find the root node of a tree.
 *
 * @param tree            The tree.
 * @return                The root node of the tree, or NULL if the tree is
 *                        empty.
 */

AVLTreeNode *avltree_rootNode(AVLTree *tree);

/**
 * Retrieve the key for a given tree node.
 *
 * @param node            The tree node.
 * @return                The key to the given node.
 */

AVLTreeKey avltree_nodeKey(AVLTreeNode *node);

/**
 * Retrieve the value at a given tree node.
 *
 * @param node            The tree node.
 * @return                The value at the given node.
 */

AVLTreeValue avltree_nodeValue(AVLTreeNode *node);

/**
 * Find the child of a given tree node.
 *
 * @param node            The tree node.
 * @param side            Which child node to get (left or right)
 * @return                The child of the tree node, or NULL if the
 *                        node has no child on the given side.
 */

AVLTreeNode *avltree_nodeChild(AVLTreeNode *node, AVLTreeNodeSide side);

/**
 * Find the parent node of a given tree node.
 *
 * @param node            The tree node.
 * @return                The parent node of the tree node, or NULL if
 *                        this is the root node.
 */

AVLTreeNode *avltree_nodeParent(AVLTreeNode *node);

/**
 * Find the height of a subtree.
 *
 * @param node            The root node of the subtree.
 * @return                The height of the subtree.
 */

int avltree_subtreeHeight(AVLTreeNode *node);

/**
 * Convert the keys in an AVL tree into a C array.  This allows
 * the tree to be used as an ordered set.
 *
 * @param tree            The tree.
 * @return                A newly allocated C array containing all the keys
 *                        in the tree, in order.  The length of the array
 *                        is equal to the number of entries in the tree
 *                        (see @ref avltree_numEntries).
 */

AVLTreeValue *avltree_toArray(AVLTree *tree);

/**
 * Retrieve the number of entries in the tree.
 *
 * @param tree            The tree.
 * @return                The number of key-value pairs stored in the tree.
 */

unsigned int avltree_numEntries(AVLTree *tree);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DSAVLTREE_H */

