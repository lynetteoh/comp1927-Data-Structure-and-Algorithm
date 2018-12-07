// graph.h ... Interface to Graph of strings
// Written by John Shepherd, September 2015
//Taken from COMP 1927lab 8 17s1
//Edited by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)

#ifndef GRAPH_H
#define GRAPH_H

typedef struct GraphRep *Graph;

// Function signatures

Graph newGraph(int);
void  disposeGraph(Graph);
int addEdge(Graph,char *,char *);
int nVertices(Graph);
int isConnected(Graph, char *, char *);
void showGraph(Graph,int);

//Helper Functions
int nE(Graph, int);
int addVertex(char *, char **, int);
char *vertexName(Graph, int);

#endif