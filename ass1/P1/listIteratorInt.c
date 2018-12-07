/*
  listIteratorInt.c ... list Iterator ADT implementation
  Written by Lean Lynn Oh 
  Student ID : z5144464
  Date: 4/4/2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorInt.h"


typedef struct Node {
  int value;
  struct Node *next;
  struct Node *prev;
} Node;


typedef struct IteratorIntRep {
  int items;
  int *data; 	            //point to value in node
  struct Node *first;
  struct Node *last;
  struct Node *cursor;
} IteratorIntRep;


//Creates new node
static Node *newNode(int v)
{
  struct Node *new;

  new = malloc(sizeof(Node));
  assert(new != NULL);
  new->value = v;
  new->prev = new->next = NULL;
  return new;
}


//Creates a new list iterator 
IteratorInt IteratorIntNew()
{
  struct IteratorIntRep *it;
  it = malloc (sizeof(struct IteratorIntRep));
  assert(it != NULL);
  it->cursor = malloc(sizeof (Node));
  assert(it->cursor != NULL);
  it->data = malloc (sizeof(int));
  assert(it->data != NULL);

  it->items = 0;
  it->first = NULL;
  it->last = NULL;
  it->data = NULL;
  it->cursor->next = it->cursor->prev = NULL;
  return it;  
}





//Resets it to the start of the list
void reset(IteratorInt it)
{

  assert(it != NULL);

  if (it->first == NULL && it->last == NULL){
    return;
  }
  
  it->cursor->next = it->first;
  it->cursor->prev = NULL;
  return;
}


//Inserts the specified value v into the list iterator 
//return 1 if success
int add(IteratorInt it, int v)
{

  assert(it != NULL);
  Node *new;
  new = newNode(v);
  assert(new != NULL);

  if (it->first == NULL){
      // empty list 
      it->first = it->last = new;
      it->cursor->prev = new;

  }else if(it->cursor->prev == it->last && it->cursor->next == NULL){
  	  //append at the end of the list
      new->prev = it->last;
      it->cursor->prev->next = new;      
      it->last = new;
      it->cursor->prev = new;

  }else if(it->cursor->next == it-> first && it->cursor->prev == NULL){  
      //insert before first node
      new->next = it->first;
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

  it->items++;
  return 1;
}


//Check list iterator has more elements to traverse in forward direction
//Returns 1 if successful  
int hasNext(IteratorInt it)
{
 
  assert(it != NULL); 

  if(it->cursor->next == NULL){
    return 0;
  }

  return 1;  
}


//Check list iterator has more elements to traverse in backward direction
//Returns 1 if successful
int hasPrevious(IteratorInt it)
{
  
  assert(it !=NULL); 

  if(it->cursor->prev == NULL){
    return 0;
  }

  return 1;
}  
   

//Advance the cursor position
//Return pointer to the next value
int *next(IteratorInt it)
{
    
  assert(it != NULL);
  

  if(it->cursor->next == NULL){
    return NULL;
  }

  it->data = &(it->cursor->next->value);
  it->cursor->prev = it->cursor->next;
  it->cursor->next = it->cursor->next->next;
  return (it->data);
}


//Move the cursor position backwards
//Return pointer to the previous value
int *previous(IteratorInt it)
{

  assert(it != NULL);
  

  if(it->cursor->prev == NULL){
   return NULL;
  }

  it->data = &(it->cursor->prev->value);
  it->cursor->next = it->cursor->prev;
  it->cursor->prev = it->cursor->prev->prev;
  return (it->data);
}


//Deletes from the list iterator the last value that was returned by next or previous or findNext or findPrevious
//Return 1 if successful 
int delete(IteratorInt it)
{
  
  assert(it != NULL);
  Node *delete;
  
  if(it->data == NULL || it->items == 0){
  	return 0;
  }

  if(it->first == it->last){
  	  delete = it->last;
      it->first = it->last = NULL;
      it->cursor->next = it->cursor->prev = NULL;

  }else if(it->data == &(it->first->value)){
  	  //delete first node
      delete = it->first;
      delete->next->prev = NULL;
      it->first = delete->next;
      it->cursor->next = it->first;
      it->cursor->prev = NULL;

  }else if(it->data == &(it->last->value)){
  	  //delete last node 
      delete = it->last;
      delete->prev->next = NULL;
      it->last = delete->prev;
      it->cursor->prev = it->last;
      it->cursor->next = NULL;

  }else if(it->data == &(it->cursor->next->value)){
  	  //delete node return by previous or findPrevious
  	  delete = it->cursor->next;
  	  delete->prev->next = it->cursor->next->next;
  	  delete->next->prev = it->cursor->prev;
      it->cursor->prev = delete->prev;
  	  it->cursor->next = it->cursor->next->next;

  }else if (it->data == &(it->cursor->prev->value)){
  	  //delete node return by next or findNext
  	  delete = it->cursor->prev;
  	  delete->prev->next = it->cursor->next;
  	  it->cursor->next->prev = delete->prev;
  	  it->cursor->prev = it->cursor->prev->prev;
      it->cursor->next = delete->next;
  
  }else {
  	  return 0;
  }
  free(delete);
  it->items--;
  return 1;
}


//Replaces the last element returned by next or previous or findNext or findPrevious with the specified element (*vp)
//Return 1 if successful
int set(IteratorInt it, int v)
{
  assert(it != NULL);
  Node *temp, *new;
  new = newNode(v);
  assert(new != NULL);


  if(it->data == NULL || it->items == 0){
    return 0;
  }
   
   if(it->first == it->last){
  	 	//replace only node
      temp = it->last;
  	  it->first = it->last = new;

   }else if(it->cursor->prev == it->first && it->data == &(it->first->value)){
      //replace first node return by next or findNext
  	  temp = it->first;
  	  new->next = it->first->next;
  	  new->prev = NULL;
  	  it->first->next->prev = new;
  	  it->first = new;
  	  it->cursor->prev = new;


  }else if(it->cursor->next == it->first && it->data == &(it->first->value)){
  	  //replace first node return by previous or findPrevious
  	  temp = it->first;
  	  new->next = it->first->next;
  	  new->prev = NULL;
  	  it->first->next->prev = new;
  	  it->first = new;
  	  it->cursor->next = new;
  	  it->cursor->prev = NULL;
  
  }else if(it->cursor->prev == it->last && it->data == &(it->last->value)){
      //replace last node return by next or findNext
      temp = it->last;
  	  new->prev = it->last->prev;
  	  new->next = NULL;
  	  it->last->prev->next = new;
  	  it->last = new;
  	  it->cursor->prev = it->last;
  	  it->cursor->next = NULL;
  
  }else if(it->cursor->next == it->last && it->data == &(it->last->value)){
  	  //replace last node return by previous or findPrevious
      temp = it->last;
  	  new->prev = it->last->prev;
  	  new->next = NULL;
  	  it->last->prev->next = new;
  	  it->last = new;
  	  it->cursor->next = it->last;

  }else if(it->data == &(it->cursor->next->value)){
      //replace node return by previous or findPrevious
  	  temp = it->cursor->next;
  	  new->next = it->cursor->next->next;
  	  new->prev = it->cursor->prev;
  	  it->cursor->prev->next = new;
  	  it->cursor->next->next->prev = new;
  	  it->cursor->next = new;

  }else if (it->data == &(it->cursor->prev->value)){
      //replace node return by next or findNext
  	  temp = it->cursor->prev;
  	  new->next = it->cursor->next;
  	  new->prev = it->cursor->prev->prev;
  	  it->cursor->next->prev = new;
  	  it->cursor->prev->prev->next = new;
  	  it->cursor->prev = new; 
  }else {
      return 0;
  }	

  free(temp);
  return 1;
}


//Find value that matches v
//Advances cursor position and return pointer to next value
int *findNext(IteratorInt it, int v)
{
  assert(it != NULL);
  Node *cursor ;					//original cursor position before searching 
  cursor = it->cursor->next;

  if(it->cursor->next == NULL){
      return NULL;
  }
   
  while (it->cursor->next != NULL && it->cursor->next->value != v){
  	  it->cursor->prev = it->cursor->next;
      it->cursor->next = it->cursor->next->next;  
  }

  if(it->cursor->next == NULL && it->cursor->prev->value != v){
      it->cursor->next = cursor;
      it->cursor->prev = cursor->prev;
      return NULL;
  }

  it->data = &(it->cursor->next->value);
  it->cursor->prev = it->cursor->next;
  it->cursor->next = it->cursor->next->next;
  return (it->data);
}

 
//Find value that matches v
//Move cursor position backwards and return pointer to previous value
int *findPrevious(IteratorInt it, int v)
{
  assert(it != NULL);

  Node *cursor;							//original cursor position before searching
  cursor = it->cursor->prev;

  if (it->cursor->prev == NULL){
    return NULL;
  }

  while (it->cursor->prev != NULL && it->cursor->prev->value != v){
      it->cursor->next = it->cursor->prev;
      it->cursor->prev = it->cursor->prev->prev;
  }

  if(it->cursor->prev == NULL && it->cursor->next->value != v){
      it->cursor->prev = cursor;
      it->cursor->next = cursor->next;
      return NULL;
  }

  it->data = &(it->cursor->prev->value);
  it->cursor->next = it->cursor->prev;
  it->cursor->prev = it->cursor->prev->prev;
  return (it->data);
}




