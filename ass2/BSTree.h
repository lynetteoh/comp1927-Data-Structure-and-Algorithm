// BSTree.h ... interface to binary search tree ADT
// Taken from COMP1927 lab 10 17s1 
// Edited by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324) to support string

#ifndef BSTREE_H
#define BSTREE_H

typedef struct BSTNode *BSTree;


// create an empty BSTree
BSTree newBSTree();
// free memory associated with BSTree
void dropBSTree(BSTree);
// display a BSTree
void showBSTree(BSTree);
// display BSTree root node
void showBSTreeNode(BSTree,FILE *);
// print values in infix order
void BSTreeInfix(BSTree,FILE *);
// count #nodes in BSTree
int BSTreeNumNodes(BSTree);
// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree, char *,char *);
// check whether a value is in a BSTree
char *BSTreeFind(BSTree, char *);
//count the number of nodes in the BSTree
int BSTreeNumNodes(BSTree );
//empty BSTree
int emptyBST(BSTree );

#endif