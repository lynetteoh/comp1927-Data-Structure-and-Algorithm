// queue.h ... interface to simple Queue of Strings
// Written by John Shepherd, September 2015
//taken from COMP 1927 lab 8 17s1 for assignment 2

#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueRep *Queue;

// Function signatures

Queue newQueue();
void disposeQueue(Queue);
void enterQueue(Queue,char *);
char *leaveQueue(Queue);
int  emptyQueue(Queue);
void showQueue(Queue q);
int queueSize(Queue );

#endif
