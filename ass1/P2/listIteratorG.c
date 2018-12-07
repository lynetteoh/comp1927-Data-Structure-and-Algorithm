/* listIteratorG.c ... Generic List Iterator implementation
	Written by Lean Lynn Oh 
	Student ID : z5144464
	Date: 4/4/2017
  
*/

/* 
    You need to submit ONLY this file.... 

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorG.h"

typedef struct Node {
	void *value;             //item stored in node
	struct Node *next;       //pointer to next node
	struct Node *prev;       //pointer to previous
  
} Node;

typedef struct IteratorGRep {
	int elems;             //numbers of items in list
	void *data;            //pointer to item stored in node 
	Node *first;           //pointer to first node
	Node *last;            //pointer to last node
	Node *cursor;          //cursor 

	ElmCompareFp 	cmpElm;
 	ElmCopyFp		copyElm;
 	ElmFreeFp 		freeElm;
} IteratorGRep;


//Creates a new list iterator with given functions to handle values
IteratorG IteratorGNew(ElmCompareFp cmp, ElmCopyFp copy, ElmFreeFp free)
{
	IteratorGRep *it;

	it = malloc (sizeof(struct IteratorGRep));
	assert(it != NULL);
	it->cursor = malloc(sizeof (Node));
  assert(it->cursor != NULL);
	it->data = malloc (sizeof(int));
	assert(it->data != NULL);

	it->elems = 0;
	it->first = NULL;
	it->last = NULL;
	it->cursor->next = NULL;
	it->cursor->prev = NULL; 
	it->cmpElm = cmp;
	it->copyElm = copy;
	it->freeElm = free;
	return it;
}


//Inserts the specified value vp into the list iterator it
//Return 1 if successful 
int  add(IteratorG it, void *vp)
{
	assert(it != NULL);
  Node *new;
  new = malloc(sizeof(Node));
 	assert(new != NULL);
  new->value = it->copyElm(vp);
  
  if (it->first == NULL){
    	// empty list 
    	it->first = it->last = new;
    	new->next = new->prev = NULL;
    	it->cursor->prev = new;

  }else if(it->cursor->prev == it->last && it->cursor->next == NULL){
  		//append at the end of the list
      	new->prev = it->last;
      	new->next = NULL;
      	it->cursor->prev->next = new;      
      	it->last = new;
      	it->cursor->prev = new;

  }else if(it->cursor->next == it-> first && it->cursor->prev == NULL){  
      	//insert before first node
      	new->next = it->first;
      	new->prev = NULL;
      	it->cursor->next->prev = new;
      	it->first = new;
      	it->cursor->prev = new;
      	it->cursor->next = new->next;

  }else if(it->cursor->prev != NULL && it->cursor->next != NULL){
  	  	//insert in between 2 nodes
      	new->prev = it->cursor->prev;
      	new->next = it->cursor->next;
   	  	it->cursor->prev->next = new;
      	it->cursor->next->prev = new;
      	it->cursor->prev = new;
      	it->cursor->next = new ->next;

  	}else{
    	return 0;
  	}

  	it->elems++;
  	return 1;
}


//Check list iterator has more elements to traverse in forward direction
//Returns 1 if successful
int  hasNext(IteratorG it)
{
	assert(it != NULL); 

  if(it->cursor->next == NULL){
   	 return 0;
  }

 	return 1;
}


//Check list iterator has more elements to traverse in backward direction
//Returns 1 if successful
int  hasPrevious(IteratorG it)
{
	assert(it !=NULL); 

 	if(it->cursor->prev == NULL){
    return 0;
  }

  return 1;

}


//Advance the cursor position
//Return pointer to the next value
void *next(IteratorG it)
{
	assert(it != NULL);

	if(it->cursor->next == NULL){
    	return NULL;
  }

  it->data = it->cursor->next->value;
  it->cursor->prev = it->cursor->next;
  it->cursor->next = it->cursor->next->next;
  return (it->data);
  
}


//Move the cursor position backwards
//Return pointer to the previous value
void *previous(IteratorG it)
{
	assert(it != NULL);

  if(it->cursor->prev == NULL){
   		return NULL;
  }

  it->data = it->cursor->prev->value;
  it->cursor->next = it->cursor->prev;
  it->cursor->prev = it->cursor->prev->prev;
  return (it->data);
}


//Deletes from the list iterator the last value that was returned by next or previous or findNext or findPrevious
//Return 1 if successful 
int  delete(IteratorG it)
{
	 assert(it != NULL);

  Node *delete;

  if(it->data == NULL|| it->elems == 0){
  		return 0;
  }

  if(it->first == it->last){
    	delete = it->last;
     	it->first = it->last = NULL;
     	it->cursor->next = it->cursor->prev = NULL;

  }else if(it->cmpElm(it->data, it->first->value) == 0){
    	//delete first node
     	delete = it->first;
     	delete->next->prev = NULL;
     	it->first = delete->next;
     	it->cursor->next = it->first;
     	it->cursor->prev = NULL;

  }else if(it->cmpElm(it->data, it->last->value) == 0){
    	//delete last node 
     	delete = it->last;
     	delete->prev->next = NULL;
     	it->last = delete->prev;
     	it->cursor->prev = it->last;
      it->cursor->next = NULL;

  }else if(it->cmpElm(it->data, it->cursor->next->value) == 0){
    	//delete node return by previous or findPrevious
    	delete = it->cursor->next;
    	delete->prev->next = it->cursor->next->next;
    	delete->next->prev = it->cursor->prev;
    	it->cursor->next = it->cursor->next->next;

 	}else if (it->cmpElm(it->data, it->cursor->prev->value) == 0){
 	  	//delete node return by next or findNext
 	  	delete = it->cursor->prev;
 	  	delete->prev->next = it->cursor->next;
 	  	it->cursor->next->prev = delete->prev;
 	  	it->cursor->prev = it->cursor->prev->prev;
  
 	}else {
 		return 0;
 	}
  it->freeElm(delete->value);
  free(delete);
  it->elems--;
  return 1;
}


//Replaces the last element returned by next or previous or findNext or findPrevious with the specified element (*vp)
//Return 1 if successful
int  set(IteratorG it, void *vp)
{
	assert(it != NULL);
  Node *temp, *new;
  new = malloc(sizeof(Node));
  assert(new != NULL);
  new->value = it->copyElm(vp);

  if(it->data == NULL || it->elems == 0){
    return 0;
  }

  if(it->first == it->last){
  	 	//replace only node
      temp = it->last;
  	 	it->first = it->last = new;

  }else if(it->cursor->prev == it->first && it->cmpElm(it->data, it->first->value) == 0){
      //replace first node return by next or findNext
      temp = it->first;
      new->next = it->first->next;
      new->prev = NULL;
      it->first->next->prev = new;
      it->first = new;
      it->cursor->prev = new;


  }else if(it->cursor->next == it->first && it->cmpElm(it->data, it->first->value) == 0){
      //replace first node return by previous or findPrevious
      temp = it->first;
      new->next = it->first->next;
      new->prev = NULL;
      it->first->next->prev = new;
      it->first = new;
      it->cursor->next = new;
      it->cursor->prev = NULL;
  
  }else if(it->cursor->prev == it->last && it->cmpElm(it->data, it->last->value) == 0){
      //replace last node return by next or findNext
      temp = it->last;
      new->prev = it->last->prev;
      it->last->prev->next = new;
      it->last = new;
      it->cursor->prev = it->last;
      it->cursor->next = NULL;
  
  }else if(it->cursor->next == it->last && it->cmpElm(it->data, it->last->value) == 0){
      //replace last node return by previous or findPrevious
      temp = it->last;
      new->prev = it->last->prev;
      new->next = NULL;
      it->last->prev->next = new;
      it->last = new;
      it->cursor->next = it->last;

 	}else if(it->cmpElm(it->data, it->cursor->next->value) == 0){
      //replace node return by previous or findPrevious
 	  	temp = it->cursor->next;
 	  	new->next = it->cursor->next->next;
 	  	new->prev = it->cursor->prev;
 	  	it->cursor->prev->next = new;
 	  	it->cursor->next->next->prev = new;
 	  	it->cursor->next = new;

  }else if (it->cmpElm(it->data, it->cursor->prev->value) == 0){
      //replace node return by next or findNext
     	temp = it->cursor->prev;
    	new->next = it->cursor->next;
    	new->prev = it->cursor->prev->prev;
    	it->cursor->next->prev = new;
    	it->cursor->prev->prev->next = new;
    	it->cursor->prev = new; 
  }	
  it->freeElm(temp->value);
  free(temp);
  return 1;
}


//Find value that matches vp
//Advances cursor position and return pointer to next value
void *findNext(IteratorG it, void *vp)
{
	assert(it != NULL);

	Node *cursor ;                         //original cursor position before searching
	cursor = it->cursor->next;

	if(it->cursor->next == NULL){
	    return NULL;
	}
	   
	while (it->cursor->next != NULL && it->cmpElm(it->cursor->next->value, vp) != 0){
	    it->cursor->prev = it->cursor->next;
	    it->cursor->next = it->cursor->next->next; 
	}

	if(it->cursor->next == NULL && it->cmpElm(it->cursor->prev->value, vp) == 0){
	    printf("cant find\n");
	    it->cursor->next = cursor;
	    it->cursor->prev = cursor->prev;
	    return NULL;
	}

  it->data = it->cursor->next->value;
  it->cursor->prev = it->cursor->next;
  it->cursor->next = it->cursor->next->next;
  printf("successful\n");


	 return (it->data);
}

//Find value that matches vp
//Move cursor position backwards and return pointer to previous value
void *findPrevious(IteratorG it, void *vp)
{
	assert(it != NULL);
  	
  	Node *cursor;                        //original cursor position before searching
  	cursor = it->cursor->prev;

	if (it->cursor->prev == NULL){
    	return NULL;
  }

  while (it->cursor->prev != NULL && it->cmpElm(it->cursor->prev->value, vp) != 0){
    	it->cursor->next = it->cursor->prev;
     	it->cursor->prev = it->cursor->prev->prev;
  }

  if(it->cursor->prev == NULL && it->cmpElm(it->cursor->next->value, vp) == 0){
    	printf("cant find\n");
    	it->cursor->prev = cursor;
      it->cursor->next = cursor->next;
      return NULL;
  }

 	it->data = it->cursor->prev->value;
	it->cursor->next = it->cursor->prev;
  it->cursor->prev = it->cursor->prev->prev;
  printf("successful\n");
  return (it->data);
}


//Resets it to the start of the list
void reset(IteratorG it)
{
	assert(it != NULL);

  if (it->first == NULL && it->last == NULL){
   	return;
  }
  	
  it->cursor->next = it->first;
  it->cursor->prev = NULL;
  return;
}


