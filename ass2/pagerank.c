//pagerank.c
//witten by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)
//to calculate pagerank of each URL and store the information (pagerank values and outgoing links) in a file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "readData.h"
#include "mergeSort.h"
#include "graph.h"
#include "queue.h"


typedef struct pr *pagerank;

//array of structure pagerank to store pagerank 
//value , degree and url name of each url
typedef struct pr{
    float value;    //page rank value
    int degree;  	//number of out degrres
    char *url;  	//url name
}pr;

/*Function Header*/
void PageRank(Graph, float, float, int);
void savePageRank(Graph, float *);
int compare (void *, void *);


int main(int argc, char *argv[]){

    //Ensure user provide correct number of inputs
    if(argc < 4){
    	fprintf(stderr, "Usage: %s [Damping Factor] [Difference Value In PageRank Sum] [Max Iterations]\n", argv[0]);
    	exit(1);
    }

    float d, diffPR;
    int maxIteration;
    d = atof(argv[1]);	// the damping factor
    diffPR = atof(argv[2]);	// the difference in PageRank sum
    maxIteration = atof(argv[3]);	// maximum iterations

    //create the data structures
    Queue q = newQueue();
    getCollection(q);
    Graph g = newGraph(queueSize(q));
    getGraph(g, q);

    //showGraph(g, 0);
    PageRank(g, d, diffPR, maxIteration);

    //free Graph
    disposeGraph(g);
    //free Queue;
    disposeQueue(q);
    return 1;
}


//calculate pagerank based on simplified pagerank algorithm provided assignment 2 specification and print it to pagerankList.txt
void PageRank(Graph g, float d, float diffPR, int maxIteration)
{
    int i, j, iteration;
    float sum, diff, temp[nVertices(g)], PR[nVertices(g)];
    pagerank *pgrank = malloc(nVertices(g)*sizeof(pagerank));

    for(i = 0; i < nVertices(g); i++){
       PR[i] = 1 / (float) nVertices(g);
    }  

    iteration = 0;
    diff = diffPR;

    while(iteration < maxIteration && diff >= diffPR){
	    iteration ++;
    	for(i = 0; i < nVertices(g); i++){
    	    temp[i] = PR[i]; //save old page rank value to get difference
    	    sum = 0;
    	    for(j = 0; j < nVertices(g); j++){   
        		if (isConnected(g, vertexName(g, j), vertexName(g, i))) //if the two pages are connected
        		    sum += PR[j] / nE(g, j);   
        	    }
        	    //pagerank formula
        	    PR[i] = (1 - d)/nVertices(g) + d*sum;
        	    diff += fabs(PR[i] - temp[i]); 
    	}
    }


    for(i = 0; i < nVertices(g); i++){ 
		pagerank newPR = malloc(sizeof(pagerank));
	    newPR->value = PR[i];
	    newPR->degree = nE(g, i);
	    newPR->url = strdup(vertexName(g, i));
	    pgrank[i] = newPR;
    }
    
    //sort in descending order
    mergeSort((void*)pgrank, 0, nVertices(g) - 1, sizeof(pagerank), compare);
   

    FILE *fp;
    if((fp = fopen("pagerankList.txt", "w")) == NULL){
        fprintf(stderr,  "Error: Cannot open pagerankList.txt \n");
        exit(1);
    }


    //print a list of urls in descending order of PageRank balues to pagerankList.txt
	for(i = 0; i < nVertices(g); i++){
	    //printf("%s, %d, %.7f\n", pgrank[i]->url, pgrank[i]->degree, pgrank[i]->value);
	    fprintf(fp, "%s, %d, %.7f\n", pgrank[i]->url, pgrank[i]->degree, pgrank[i]->value);
    }

    //free memory
    for(i = 0; i < nVertices(g); i++){
    	 free(pgrank[i]);
    }
    
    free(pgrank);	//free memory
	fclose(fp);  //close file
}

// to compare the page rank values to sort the output
int compare (void *a, void *b)
{
    if((((pagerank)b)->value - ((pagerank)a)->value ) > 0.0000001 ){
        return 0;
    }
    //the difference between 2 pageranks value must be very small
    return 1;
}
