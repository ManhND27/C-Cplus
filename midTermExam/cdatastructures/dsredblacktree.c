#include <stdlib.h>

#include "dsredblacktree.h"


struct _RBTreeNode {
	RBTreeNodeColor color;
	RBTreeKey key;
	RBTreeValue value;
	RBTreeNode *parent;
	RBTreeNode *children[2];
};

struct _RBTree {
    RBTreeNode *rootNode;
    RBTreeCompareFunc compareFunc;
    int numNodes;
};

static RBTreeNodeSide rbtree_nodeSide(RBTreeNode *node)
{
	if (node->parent->children[RB_TREE_NODE_LEFT] == node) {
		return RB_TREE_NODE_LEFT;
	} else {
		return RB_TREE_NODE_RIGHT;
	}
}

static RBTreeNode *rbtree_nodeSibling(RBTreeNode *node)
{
    RBTreeNodeSide side = rbtree_nodeSide(node);

	return node->parent->children[1 - side];
}

RBTreeNode *rbtree_nodeUncle(RBTreeNode *node)
{
    return rbtree_nodeSibling(node->parent);
}

/* Replace node1 with node2 at its parent. */

static void rbtree_nodeReplace(RBTree *tree,
                               RBTreeNode *node1,
                               RBTreeNode *node2)
{
	/* Set the node's parent pointer. */

	if (node2 != NULL) {
		node2->parent = node1->parent;
	}

	/* The root node? */

	if (node1->parent == NULL) {
        tree->rootNode = node2;
	} else {
        int side = rbtree_nodeSide(node1);
		node1->parent->children[side] = node2;
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

static RBTreeNode *rbtree_rotate(RBTree *tree,
                                 RBTreeNode *node,
                                 RBTreeNodeSide direction)
{
	/* The child of this node will take its place:
	   for a left rotation, it is the right child, and vice versa. */

    RBTreeNode *newRoot = node->children[1-direction];

	/* Make new_root the root, update parent pointers. */

    rbtree_nodeReplace(tree, node, newRoot);

	/* Rearrange pointers */

    node->children[1-direction] = newRoot->children[direction];
    newRoot->children[direction] = node;

	/* Update parent references */

    node->parent = newRoot;

	if (node->children[1-direction] != NULL) {
		node->children[1-direction]->parent = node;
	}

    return newRoot;
}


RBTree *rbtree_new(RBTreeCompareFunc compareFunc)
{
    RBTree *newTree = malloc(sizeof(RBTree));

    if (newTree == NULL) {
		return NULL;
	}

    newTree->rootNode = NULL;
    newTree->numNodes = 0;
    newTree->compareFunc = compareFunc;

    return newTree;
}

static void rbtree_freeSubtree(RBTreeNode *node)
{
	if (node != NULL) {
		/* Recurse to subnodes */

        rbtree_freeSubtree(node->children[RB_TREE_NODE_LEFT]);
        rbtree_freeSubtree(node->children[RB_TREE_NODE_RIGHT]);

		/* Free this node */

		free(node);
	}
}

void rbtree_free(RBTree *tree)
{
	/* Free all nodes in the tree */

    rbtree_freeSubtree(tree->rootNode);

	/* Free back the main tree structure */

	free(tree);
}

static void rbtree_insertCase1(RBTree *tree, RBTreeNode *node);
static void rbtree_insertCase2(RBTree *tree, RBTreeNode *node);
static void rbtree_insertCase3(RBTree *tree, RBTreeNode *node);
static void rbtree_insertCase4(RBTree *tree, RBTreeNode *node);
static void rbtree_insertCase5(RBTree *tree, RBTreeNode *node);

/* Insert case 1: If the new node is at the root of the tree, it must
 * be recolored black, as the root is always black. */

static void rbtree_insertCase1(RBTree *tree, RBTreeNode *node)
{
	if (node->parent == NULL) {

		/* The root node is black */

		node->color = RB_TREE_NODE_BLACK;

	} else {

		/* Not root */

        rbtree_insertCase2(tree, node);
	}
}

/* Insert case 2: If the parent of the new node is red, this
 * conflicts with the red-black tree conditions, as both children
 * of every red node are black. */

static void rbtree_insertCase2(RBTree *tree, RBTreeNode *node)
{
	/* Note that if this function is being called, we already know
	 * the node has a parent, as it is not the root node. */

	if (node->parent->color != RB_TREE_NODE_BLACK) {
        rbtree_insertCase3(tree, node);
	}
}

/* Insert case 3: If the parent and uncle are both red, repaint them
 * both black and repaint the grandparent red.  */

static void rbtree_insertCase3(RBTree *tree, RBTreeNode *node)
{
	/* Note that the node must have a grandparent, as the parent
	 * is red, and the root node is always black. */

    RBTreeNode *grandparent = node->parent->parent;
    RBTreeNode *uncle = rbtree_nodeUncle(node);

	if (uncle != NULL && uncle->color == RB_TREE_NODE_RED) {

		node->parent->color = RB_TREE_NODE_BLACK;
		uncle->color = RB_TREE_NODE_BLACK;
		grandparent->color = RB_TREE_NODE_RED;

		/* Recurse to grandparent */

        rbtree_insertCase1(tree, grandparent);

	} else {
        rbtree_insertCase4(tree, node);
	}
}

/* Case 4: If the parent is red, but the uncle is black, we need to do
 * some rotations to keep the tree balanced and complying with the tree
 * conditions.  If the node is on the opposite side relative to its parent
 * as the parent is relative to its grandparent, rotate around the
 * parent.  Either way, we will continue to case 5.
 *
 * eg.
 *
 *         B                              B
 *        / \                            / \
 *       R   B          ->     node ->  R   B
 *        \                            /
 *         R  <- node                 R
 *
 */

void rbtree_insertCase4(RBTree *tree, RBTreeNode *node)
{
	/* Note that at this point, by implication from case 3, we know
	 * that the parent is red, but the uncle is black.  We therefore
	 * only need to examine what side the node is on relative
	 * to its parent, and the side the parent is on relative to
	 * the grandparent. */

    RBTreeNode *nextNode;
    RBTreeNodeSide side = rbtree_nodeSide(node);

    if (side != rbtree_nodeSide(node->parent)) {

		/* After the rotation, we will continue to case 5, but
		 * the parent node will be at the bottom. */

        nextNode = node->parent;

		/* Rotate around the parent in the opposite direction
		 * to side. */

        rbtree_rotate(tree, node->parent, 1-side);
	} else {
        nextNode = node;
	}

    rbtree_insertCase5(tree, nextNode);
}

/* Case 5: The node is on the same side relative to its parent as the
 * parent is relative to its grandparent.  The node and its parent are
 * red, but the uncle is black.
 *
 * Now, rotate at the grandparent and recolor the parent and grandparent
 * to black and red respectively.
 *
 *               G/B                 P/B
 *              /   \               /   \
 *           P/R     U/B    ->   N/R     G/R
 *          /   \                       /   \
 *       N/R      ?                   ?      U/B
 *
 */

void rbtree_insertCase5(RBTree *tree, RBTreeNode *node)
{
    RBTreeNode *parent = node->parent;
    RBTreeNode *grandparent = parent->parent;

	/* What side are we, relative to the parent?  This will determine
	 * the direction that we rotate. */

    RBTreeNodeSide side = rbtree_nodeSide(node);

	/* Rotate at the grandparent, in the opposite direction to side. */

    rbtree_rotate(tree, grandparent, 1-side);

	/* Recolor the (old) parent and grandparent. */

	parent->color = RB_TREE_NODE_BLACK;
	grandparent->color = RB_TREE_NODE_RED;
}

RBTreeNode *rbtree_insert(RBTree *tree, RBTreeKey key, RBTreeValue value)
{
	/* Allocate a new node */

    RBTreeNode *node = malloc(sizeof(RBTreeNode));
	if (node == NULL) {
		return NULL;
	}

	/* Set up structure.  Initially, the node is red. */

	node->key = key;
	node->value = value;
	node->color = RB_TREE_NODE_RED;
	node->children[RB_TREE_NODE_LEFT] = NULL;
	node->children[RB_TREE_NODE_RIGHT] = NULL;

	/* First, perform a normal binary tree-style insert. */

    RBTreeNode *parent = NULL;
    RBTreeNode **rover = &tree->rootNode;
    RBTreeNodeSide side;

	while (*rover != NULL) {

		/* Update parent */

		parent = *rover;

		/* Choose which path to go down, left or right child */

        if (tree->compareFunc(value, (*rover)->value) < 0) {
			side = RB_TREE_NODE_LEFT;
		} else {
			side = RB_TREE_NODE_RIGHT;
		}

		rover = &(*rover)->children[side];
	}

	/* Insert at the position we have reached */

	*rover = node;
	node->parent = parent;

	/* Possibly reorder the tree. */

    rbtree_insertCase1(tree, node);

	/* Update the node count */

    ++tree->numNodes;

	return node;
}

RBTreeNode *rbtree_lookupNode(RBTree *tree, RBTreeKey key)
{
    RBTreeNode *node = tree->rootNode;
    RBTreeNodeSide side;
    int diff;

	/* Search down the tree. */

	while (node != NULL) {
        diff = tree->compareFunc(key, node->key);

		if (diff == 0) {
			return node;
		} else if (diff < 0) {
			side = RB_TREE_NODE_LEFT;
		} else {
			side = RB_TREE_NODE_RIGHT;
		}

		node = node->children[side];
	}

	/* Not found. */

	return NULL;
}

RBTreeValue rbtree_lookup(RBTree *tree, RBTreeKey key)
{
	/* Find the node for this key. */

    RBTreeNode *node = rbtree_lookupNode(tree, key);

	if (node == NULL) {
		return RB_TREE_NULL;
	} else {
		return node->value;
	}
}

void rbtree_removeNode(RBTree *tree, RBTreeNode *node)
{
	/* TODO */
}

int rbtree_remove(RBTree *tree, RBTreeKey key)
{
	/* Find the node to remove. */

    RBTreeNode *node = rbtree_lookupNode(tree, key);

	if (node == NULL) {
		return 0;
	}

    rbtree_removeNode(tree, node);

	return 1;
}

RBTreeNode *rbtree_rootNode(RBTree *tree)
{
    return tree->rootNode;
}

RBTreeKey rbtree_nodeKey(RBTreeNode *node)
{
	return node->key;
}

RBTreeValue rbtree_nodeValue(RBTreeNode *node)
{
	return node->value;
}

RBTreeNode *rbtree_nodeChild(RBTreeNode *node, RBTreeNodeSide side)
{
	if (side == RB_TREE_NODE_LEFT || side == RB_TREE_NODE_RIGHT) {
		return node->children[side];
	} else {
		return NULL;
	}
}

RBTreeNode *rbtree_nodeParent(RBTreeNode *node)
{
	return node->parent;
}

RBTreeValue *rbtree_toArray(RBTree *tree)
{
	/* TODO */
	return NULL;
}

int rbtree_numEntries(RBTree *tree)
{
    return tree->numNodes;
}

