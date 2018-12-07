// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015
//Taken from lab8 17s1
//Edited by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)
//Added function: intersection - to get intersection between two different sets
//					copySet - to get all elements in Set

#ifndef SET_H
#define SET_H

typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);
void showSet(Set);
Set intersection(Set, Set, Set);
int emptySet (Set);
char ** copySet(Set);
#endif
