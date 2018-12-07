//readData.h
//Written by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)

#ifndef READDATA_H
#define READDATA_H

#include "graph.h"
#include "queue.h"
#include "BSTree.h"

Queue getCollection(Queue);
Graph getGraph(Graph, Queue);
BSTree buildBSTree(BSTree, Queue);


#endif
