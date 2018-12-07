//inverted.c
//written by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)
//reads data from a given collection of pages in collection.txt and generates an "inverted index" that provides a list (set) of urls for every word in a given collection of pages

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BSTree.h"
#include "queue.h"
#include "readData.h"
#include "normalise.h"

//size for filenames
#define SIZE 50

//* Function Header */
void printIndex(BSTree);

//main function 
int main(int argc, char *argv[]){
    Queue q = newQueue();
    getCollection(q); //store pages
    //showQueue(q);   //print Queue to stdout
    BSTree bst = newBSTree(); //create a BST
    bst = buildBSTree(bst, q); //fill tree with pages

    printIndex(bst);	//print to a file

    disposeQueue(q); //save memory
    dropBSTree(bst);   //save memory
    return 1;
}

//get filename from queue
//read files to get keywords
//stores keyword and their respective url 
BSTree buildBSTree(BSTree BST, Queue q){
  char curr[SIZE];
  char filename[SIZE];
  char buff[SIZE];
  FILE *fp;
  
  while(!emptyQueue(q)){
    strcpy(curr, leaveQueue(q));
    strcpy(filename, curr);
    strcat(filename,".txt");

    if ((fp = fopen(filename, "r")) == NULL){
      fprintf(stderr, "Can't open file %s\n", filename);
      exit(1);
    }

    while(fscanf(fp, "%s", buff) != EOF){
      if(strcmp(buff, "Section-2") == 0){  //check for given keyword
        fscanf(fp,"%s",buff);
        break;
      }
    }

    //insert URL in BST tree with keyword as the key
    while(strcmp(buff, "#end") != 0){
      if (BST != NULL)   
        BSTreeInsert(BST, normalise(buff), normalise(curr));    
      else 
        BST = BSTreeInsert(BST, normalise(buff), normalise(curr));  
      fscanf(fp,"%s",buff);
    }

   
    fclose(fp);
  }  
  return BST;
}

//prints the BST in infix order
//prints all the data to invertedIndex.txt in alphabetical order
void printIndex(BSTree BST){
    
    FILE *fp;
  
    if((fp = fopen("invertedIndex.txt", "w")) == NULL){
    	fprintf(stderr, "Can't open file : InvertedIndex.txt\n");
      return exit(1);
    }
    //BSTreeInfix(BST, stdout); //print to STDOUT
  	BSTreeInfix(BST, fp);      //print to file
    fclose(fp);    
    return;
}
