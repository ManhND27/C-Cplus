#include <stdlib.h>

#include "dsavltree.h"


/* AVL Tree (balanced binary search tree) */

struct _AVLTreeNode {
	AVLTreeNode *children[2];
	AVLTreeNode *parent;
	AVLTreeKey key;
	AVLTreeValue value;
	int height;
};

struct _AVLTree {
    AVLTreeNode *rootNode;
    AVLTreeCompareFunc compareFunc;
    unsigned int numNodes;
};

AVLTree *avltree_new(AVLTreeCompareFunc compareFunc)
{
    AVLTree *newTree = (AVLTree *) malloc(sizeof(AVLTree));

    if (newTree == NULL) {
		return NULL;
	}

    newTree->rootNode = NULL;
    newTree->compareFunc = compareFunc;
    newTree->numNodes = 0;

    return newTree;
}

static void avltree_freeSubtree(AVLTree *tree, AVLTreeNode *node)
{
	if (node == NULL) {
		return;
	}

    avltree_freeSubtree(tree, node->children[AVL_TREE_NODE_LEFT]);
    avltree_freeSubtree(tree, node->children[AVL_TREE_NODE_RIGHT]);

	free(node);
}

void avltree_free(AVLTree *tree)
{
	/* Destroy all nodes */

    avltree_freeSubtree(tree, tree->rootNode);

	/* Free back the main tree data structure */

	free(tree);
}

int avltree_subtreeHeight(AVLTreeNode *node)
{
	if (node == NULL) {
		return 0;
	} else {
		return node->height;
	}
}

/* Update the "height" variable of a node, from the heights of its
 * children.  This does not update the height variable of any parent
 * nodes. */

static void avltree_updateHeight(AVLTreeNode *node)
{
    AVLTreeNode *leftSubtree = node->children[AVL_TREE_NODE_LEFT];
    AVLTreeNode *rightSubtree = node->children[AVL_TREE_NODE_RIGHT];

    int leftHeight = avltree_subtreeHeight(leftSubtree);
    int rightHeight = avltree_subtreeHeight(rightSubtree);

    if (leftHeight > rightHeight) {
        node->height = leftHeight + 1;
	} else {
        node->height = rightHeight + 1;
	}
}

/* Find what side a node is relative to its parent */

static AVLTreeNodeSide avltree_nodeParentSide(AVLTreeNode *node)
{
	if (node->parent->children[AVL_TREE_NODE_LEFT] == node) {
		return AVL_TREE_NODE_LEFT;
	} else {
		return AVL_TREE_NODE_RIGHT;
	}
}

/* Replace node1 with node2 at its parent. */

static void avltree_nodeReplace(AVLTree *tree,
                                AVLTreeNode *node1,
                                AVLTreeNode *node2)
{
	/* Set the node's parent pointer. */

	if (node2 != NULL) {
		node2->parent = node1->parent;
	}

	/* The root node? */

    int side;

	if (node1->parent == NULL) {
        tree->rootNode = node2;
	} else {
        side = avltree_nodeParentSide(node1);
		node1->parent->children[side] = node2;

        avltree_updateHeight(node1->parent);
	}
}

/* Rotate a section of the tree.  'node' is the node at the top
 * of the section to be rotated.  'direction' is the direction in
 * which to rotate the tree: left or right, as shown in the following
 * diagram:
 *
 * Left rotation:              Right rotation:
 *
 *      B                             D
 *     / \                           / \
 *    A   D                         B   E
 *       / \                       / \
 *      C   E                     A   C

 * is rotated to:              is rotated to:
 *
 *        D                           B
 *       / \                         / \
 *      B   E                       A   D
 *     / \                             / \
 *    A   C                           C   E
 */

static AVLTreeNode *avl_treeRotate(AVLTree *tree,
                                   AVLTreeNode *node,
                                   AVLTreeNodeSide direction)
{
	/* The child of this node will take its place:
	   for a left rotation, it is the right child, and vice versa. */

    AVLTreeNode *newRoot = node->children[1-direction];

	/* Make new_root the root, update parent pointers. */

    avltree_nodeReplace(tree, node, newRoot);

	/* Rearrange pointers */

    node->children[1-direction] = newRoot->children[direction];
    newRoot->children[direction] = node;

	/* Update parent references */

    node->parent = newRoot;

	if (node->children[1-direction] != NULL) {
		node->children[1-direction]->parent = node;
	}

	/* Update heights of the affected nodes */

    avltree_updateHeight(newRoot);
    avltree_updateHeight(node);

    return newRoot;
}


/* Balance a particular tree node.
 *
 * Returns the root node of the new subtree which is replacing the
 * old one. */

static AVLTreeNode *avltree_nodeBalance(AVLTree *tree, AVLTreeNode *node)
{
    AVLTreeNode *leftSubtree = node->children[AVL_TREE_NODE_LEFT];
    AVLTreeNode *rightSubtree = node->children[AVL_TREE_NODE_RIGHT];

	/* Check the heights of the child trees.  If there is an unbalance
	 * (difference between left and right > 2), then rotate nodes
	 * around to fix it */

    AVLTreeNode *child;
    int diff = avltree_subtreeHeight(rightSubtree) - avltree_subtreeHeight(leftSubtree);

	if (diff >= 2) {

		/* Biased toward the right side too much. */

        child = rightSubtree;

		if (avltree_subtreeHeight(child->children[AVL_TREE_NODE_RIGHT])
            < avltree_subtreeHeight(child->children[AVL_TREE_NODE_LEFT])) {

			/* If the right child is biased toward the left
			 * side, it must be rotated right first (double
			 * rotation) */

            avl_treeRotate(tree, rightSubtree, AVL_TREE_NODE_RIGHT);
		}

		/* Perform a left rotation.  After this, the right child will
		 * take the place of this node.  Update the node pointer. */

        node = avl_treeRotate(tree, node, AVL_TREE_NODE_LEFT);

	} else if (diff <= -2) {

		/* Biased toward the left side too much. */

		child = node->children[AVL_TREE_NODE_LEFT];

		if (avltree_subtreeHeight(child->children[AVL_TREE_NODE_LEFT])
            < avltree_subtreeHeight(child->children[AVL_TREE_NODE_RIGHT])) {

			/* If the left child is biased toward the right
			 * side, it must be rotated right left (double
			 * rotation) */

            avl_treeRotate(tree, leftSubtree, AVL_TREE_NODE_LEFT);
		}

		/* Perform a right rotation.  After this, the left child will
		 * take the place of this node.  Update the node pointer. */

        node = avl_treeRotate(tree, node, AVL_TREE_NODE_RIGHT);
	}

	/* Update the height of this node */

    avltree_updateHeight(node);

	return node;
}

/* Walk up the tree from the given node, performing any needed rotations */

static void avltree_balanceToRoot(AVLTree *tree, AVLTreeNode *node)
{
    AVLTreeNode *rover = node;

	while (rover != NULL) {

		/* Balance this node if necessary */

        rover = avltree_nodeBalance(tree, rover);

		/* Go to this node's parent */

		rover = rover->parent;
	}
}

AVLTreeNode *avltree_insert(AVLTree *tree, AVLTreeKey key, AVLTreeValue value)
{
	/* Walk down the tree until we reach a NULL pointer */

    AVLTreeNode **rover = &tree->rootNode;
    AVLTreeNode *previousNode = NULL;

	while (*rover != NULL) {
        previousNode = *rover;
        if (tree->compareFunc(key, (*rover)->key) < 0) {
			rover = &((*rover)->children[AVL_TREE_NODE_LEFT]);
		} else {
			rover = &((*rover)->children[AVL_TREE_NODE_RIGHT]);
		}
	}

	/* Create a new node.  Use the last node visited as the parent link. */

    AVLTreeNode *newNode = (AVLTreeNode *) malloc(sizeof(AVLTreeNode));

    if (newNode == NULL) {
		return NULL;
	}

    newNode->children[AVL_TREE_NODE_LEFT] = NULL;
    newNode->children[AVL_TREE_NODE_RIGHT] = NULL;
    newNode->parent = previousNode;
    newNode->key = key;
    newNode->value = value;
    newNode->height = 1;

	/* Insert at the NULL pointer that was reached */

    *rover = newNode;

	/* Rebalance the tree, starting from the previous node. */

    avltree_balanceToRoot(tree, previousNode);

	/* Keep track of the number of entries */

    ++tree->numNodes;

    return newNode;
}

/* Find the nearest node to the given node, to replace it.
 * The node returned is unlinked from the tree.
 * Returns NULL if the node has no children. */

static AVLTreeNode *avltree_nodeGetReplacement(AVLTree *tree, AVLTreeNode *node)
{
    AVLTreeNode *leftSubtree = node->children[AVL_TREE_NODE_LEFT];
    AVLTreeNode *rightSubtree = node->children[AVL_TREE_NODE_RIGHT];

	/* No children? */

    if (leftSubtree == NULL && rightSubtree == NULL) {
		return NULL;
	}

	/* Pick a node from whichever subtree is taller.  This helps to
	 * keep the tree balanced. */

    int leftHeight = avltree_subtreeHeight(leftSubtree);
    int rightHeight = avltree_subtreeHeight(rightSubtree);
    int side;

    if (leftHeight < rightHeight) {
		side = AVL_TREE_NODE_RIGHT;
	} else {
		side = AVL_TREE_NODE_LEFT;
	}

	/* Search down the tree, back towards the center. */

    AVLTreeNode *result = node->children[side];

	while (result->children[1-side] != NULL) {
		result = result->children[1-side];
	}

	/* Unlink the result node, and hook in its remaining child
	 * (if it has one) to replace it. */

    AVLTreeNode *child = result->children[side];
    avltree_nodeReplace(tree, result, child);

	/* Update the subtree height for the result node's old parent. */

    avltree_updateHeight(result->parent);

	return result;
}

/* Remove a node from a tree */

void avltree_removeNode(AVLTree *tree, AVLTreeNode *node)
{
	/* The node to be removed must be swapped with an "adjacent"
	 * node, ie. one which has the closest key to this one. Find
	 * a node to swap with. */

    AVLTreeNode *swapNode = avltree_nodeGetReplacement(tree, node);
    AVLTreeNode *balanceStartpoint;

    if (swapNode == NULL) {

		/* This is a leaf node and has no children, therefore
		 * it can be immediately removed. */

		/* Unlink this node from its parent. */

        avltree_nodeReplace(tree, node, NULL);

		/* Start rebalancing from the parent of the original node */

        balanceStartpoint = node->parent;

	} else {
		/* We will start rebalancing from the old parent of the
		 * swap node.  Sometimes, the old parent is the node we
		 * are removing, in which case we must start rebalancing
		 * from the swap node. */

        if (swapNode->parent == node) {
            balanceStartpoint = swapNode;
		} else {
            balanceStartpoint = swapNode->parent;
		}

		/* Copy references in the node into the swap node */

        int i;

		for (i=0; i<2; ++i) {
            swapNode->children[i] = node->children[i];

            if (swapNode->children[i] != NULL) {
                swapNode->children[i]->parent = swapNode;
			}
		}

        swapNode->height = node->height;

		/* Link the parent's reference to this node */

        avltree_nodeReplace(tree, node, swapNode);
	}

	/* Destroy the node */

	free(node);

	/* Keep track of the number of nodes */

    --tree->numNodes;

	/* Rebalance the tree */

    avltree_balanceToRoot(tree, balanceStartpoint);
}

/* Remove a node by key */

int avltree_remove(AVLTree *tree, AVLTreeKey key)
{
	/* Find the node to remove */

    AVLTreeNode *node = avltree_lookupNode(tree, key);

	if (node == NULL) {
		/* Not found in tree */

		return 0;
	}

	/* Remove the node */

	avltree_removeNode(tree, node);

	return 1;
}

AVLTreeNode *avltree_lookupNode(AVLTree *tree, AVLTreeKey key)
{
	/* Search down the tree and attempt to find the node which
	 * has the specified key */

    AVLTreeNode *node = tree->rootNode;
    int diff;

	while (node != NULL) {

        diff = tree->compareFunc(key, node->key);

		if (diff == 0) {

			/* Keys are equal: return this node */

			return node;

		} else if (diff < 0) {
			node = node->children[AVL_TREE_NODE_LEFT];
		} else {
			node = node->children[AVL_TREE_NODE_RIGHT];
		}
	}

	/* Not found */

	return NULL;
}

AVLTreeValue avltree_lookup(AVLTree *tree, AVLTreeKey key)
{
	/* Find the node */

    AVLTreeNode *node = avltree_lookupNode(tree, key);

	if (node == NULL) {
		return AVL_TREE_NULL;
	} else {
		return node->value;
	}
}

AVLTreeNode *avltree_rootNode(AVLTree *tree)
{
    return tree->rootNode;
}

AVLTreeKey avltree_nodeKey(AVLTreeNode *node)
{
	return node->key;
}

AVLTreeValue avltree_nodeValue(AVLTreeNode *node)
{
	return node->value;
}

AVLTreeNode *avltree_nodeChild(AVLTreeNode *node, AVLTreeNodeSide side)
{
	if (side == AVL_TREE_NODE_LEFT || side == AVL_TREE_NODE_RIGHT) {
		return node->children[side];
	} else {
		return NULL;
	}
}

AVLTreeNode *avltree_nodeParent(AVLTreeNode *node)
{
	return node->parent;
}

unsigned int avltree_numEntries(AVLTree *tree)
{
    return tree->numNodes;
}

static void avltree_toArrayAddSubtree(AVLTreeNode *subtree,
                                      AVLTreeValue *array,
                                      int *index)
{
	if (subtree == NULL) {
		return;
	}

	/* Add left subtree first */

    avltree_toArrayAddSubtree(subtree->children[AVL_TREE_NODE_LEFT],
                              array,
                              index);

	/* Add this node */

	array[*index] = subtree->key;
	++*index;

	/* Finally add right subtree */

    avltree_toArrayAddSubtree(subtree->children[AVL_TREE_NODE_RIGHT],
                              array,
                              index);
}

AVLTreeValue *avltree_toArray(AVLTree *tree)
{
	/* Allocate the array */

    AVLTreeValue *array = malloc(sizeof(AVLTreeValue) * tree->numNodes);
    int index;

	if (array == NULL) {
		return NULL;
	}

	index = 0;

	/* Add all keys */

    avltree_toArrayAddSubtree(tree->rootNode, array, &index);

	return array;
}

