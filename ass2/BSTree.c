// BSTree.c ... implementation of binary search tree ADT
//Taken from COMP1927 lab10 17s1
//Edited by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324) where values of the BST are a linked list


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "BSTree.h"

/* node for the linked list */
typedef struct listNode *listLink;

typedef struct listNode {
    char *url;
    listLink prev;
    listLink next;  // link to the next node
} listNode;

/* the node for the BST tree */
typedef struct BSTNode *BSTLink;
typedef struct BSTNode {
    char *keyword;          //keyword
    listLink urlList;       //linked list of webpages
    BSTLink left, right;    //left right nodes
} BSTNode;

/* function headers */
BSTLink newBSTNode(char *, char *);
listLink newListNode(char *);
//free memory associated with linked list
void freeList(listLink );
//display linked list node
void showListNode(listLink, FILE *);
//add node in linked list in sorted order
void insertNode(BSTree, listLink , char *);


// make a new node containing a value
BSTLink newBSTNode(char *word, char *webPage)
{
    BSTLink new = malloc(sizeof(BSTNode));
    assert(new != NULL);
    new->urlList = newListNode(webPage);
    new->keyword = strdup(word);
    new->left = new->right = NULL;
    return new;
}

//creates a new link node
listLink newListNode(char *webPage){
    listLink n = malloc(sizeof(listNode));
    assert(n != NULL);
    n->url = strdup(webPage);
    n->next = n->prev = NULL; 
    return n;
}

// create a new empty BSTree
BSTree newBSTree()
{
    return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t)
{
    if (t == NULL) return;
    dropBSTree(t->left);
    dropBSTree(t->right);
    freeList(t->urlList);
    free(t->keyword);
    free(t);
}

//free memory associated with the linked list
void freeList(listLink L)
{
    if(L == NULL) return;
    freeList(L->next);
    free(L->url);
    free(L);
}

void showLinkNode(listLink L, FILE *out){
    if (L == NULL) return;
    //printf(" %s", L->url);
    showLinkNode(L->next, out);
    fprintf(out, " %s", L->url);
}

// display BSTree root node
void showBSTreeNode(BSTree t, FILE *out)
{
    if (t == NULL) return;
    fprintf(out, "%s", t->keyword);
    //printf("\n%s", t->keyword);
    showLinkNode(t->urlList, out);
    fprintf(out, "\n");
}

// print values in infix order
void BSTreeInfix(BSTree t, FILE *out)
{
    if (t == NULL) return;
    BSTreeInfix(t->left, out);
    showBSTreeNode(t, out);
    BSTreeInfix(t->right, out);
}

// count number of nodes in BSTree
int BSTreeNumNodes(BSTree t)
{
    if (t == NULL)
        return 0;
    else
        return 1 + BSTreeNumNodes(t->left)
                 + BSTreeNumNodes(t->right);
}


// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree t, char *word, char *webPage)
{
    if (t == NULL)
	   return newBSTNode(word, webPage);
    else if (strcmp(word, t->keyword) < 0)
	   t->left = BSTreeInsert(t->left, word, webPage);
    else if (strcmp(word, t->keyword) > 0)
	   t->right = BSTreeInsert(t->right, word, webPage);
    else // (v == t->value)
	   insertNode(t, t->urlList, webPage);
    /* don't insert duplicates */;
    return t;
}

//adds item to the link list in the BST tree (per node)
//and ensure that it is sorted so that it prints
//nicely when ./inverted is run
void insertNode(BSTree t, listLink L, char *webPage)
{
    listLink n = newListNode(webPage);

    if (L == NULL){
        //empty link 
	    t->urlList = n;
        return;
    } else if (strcmp(L->url, webPage) < 0){
        //smallest value
        n->next = L;
        t->urlList = n;
        return;
    } else if (strcmp(L->url, webPage) == 0){
        //if smallest value exists in the list
        return;         
    } else {
        while(L->next != NULL){ //tranverse the list
            if ((strcmp(L->url, webPage) > 0) && (strcmp(L->next->url, webPage) < 0)){
            //somewhere in the middle of the list
                n->next = L->next;
                n->prev = L->prev;
                L->next = n;
                return;
            }else if(strcmp(L->url, webPage) == 0){
                //exist in the list
                return;
            }
            L = L->next;
        }
        //largest value 
        if(strcmp(L->url, webPage) > 0){
            n->prev = L;
            L->next = n;
        }
        return;
    }
            
}

