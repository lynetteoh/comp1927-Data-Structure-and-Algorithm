// readData.c
// Written by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)
//read data from file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "readData.h"
#include "normalise.h"
#include "queue.h"
#include "graph.h"
#include "set.h"

//size for filenames
#define SIZE 50


//get data from collection.txt
Queue getCollection(Queue q){
   char buff[SIZE];
    FILE *fp;
    Set seen = newSet(); 
    
    if((fp = fopen("collection.txt", "r")) == NULL){
      fprintf(stderr, "Can't open file: collection.txt \n");
      exit(1);
    }

    while(fscanf(fp, "%s", buff) != EOF){
        if (!isElem(seen, buff)){ //ignore duplicate links
            enterQueue(q, buff);
            insertInto(seen, buff);
        } 
      
    }
    
    disposeSet(seen);  //clear memory
    fclose(fp);     //dispose of pointer
    return q;
}

//create graph to show outgoing links
Graph getGraph(Graph g, Queue q)
{
    char curr[SIZE];
    char buff[SIZE];
    char filename[SIZE];
    FILE *fp;
    while(!emptyQueue(q)){   //while there are still links left to check
    	strcpy(curr, leaveQueue(q)); //get next url
      strcpy(filename, curr);
      strcat(filename,".txt");

      if ((fp = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Can't open file %s\n", filename);
        exit(1);
      }

      while(fscanf(fp, "%s", buff) != EOF){
        if(strcmp(buff, "Section-1") == 0){  //check for given keyword
          fscanf(fp,"%s",buff);
          break;
        }
      }

      while(strcmp(buff, "#end") != 0){  //check again for given keyword
        addEdge(g, curr, buff);
        fscanf(fp, "%s", buff);
      }

    	fclose(fp);
    }
    return g;
}




