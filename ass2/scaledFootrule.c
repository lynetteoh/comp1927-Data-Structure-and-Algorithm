//scaledFootrule.c
//Written by Lean Lynn OH (z5144464) & Chun Lam Lum (z5144464) 
//Aggregates ranks from files given as commandline arguments, and output aggregated rank list with minimum scaled footrule distance.

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "set.h"
#include "queue.h"

#define SIZE 254


//function header
int getRank(char *, char ***);
void swap(int *, int, int );
void permutation(int *, int, int, int *, int, char **, double *, int *,char ***);
double calScaledFootrule(int, int *, char ***, char**, int *, int);
int getIndex(char *, char **, int );



int main(int argc, char *argv[])
{
	
	char ***rankTable = malloc(sizeof(char *)*argc-1); // 2d array
	int i, j, columnSize[argc-1], unionSize, pos; //number of URL in a file
	Set unionS = newSet();
	double totalScaledFootrule = DBL_MAX;  //large number for comparison in permutation

	if(argc < 2){  //makes sure there is atleast one keyword
  		fprintf(stderr, "Usage: %s [filenames]", argv[0]);
  		exit(1);
    }

    /*  read through rank files
    	create a 2d array with the url name and postion
		insert url into set 
		permute to get P position 
		calculate scaled Footrule with the respective P position
		print the minimum scaled Footrule

	*/
  	for (i = 0; i < (argc-1); i++) {		
		columnSize[i] = getRank(argv[i+1], &rankTable[i]); //read through the file. store number of urls in rankTable[i]
        for(j = 0; j < columnSize[i]; j++){
        	insertInto(unionS, rankTable[i][j]); //insert every member of array into set to get rid of duplicates	
            
        }
    }

    //showSet(unionS);
    unionSize = nElems(unionS);
    int position[unionSize];
    int finalP[unionSize];
    char **URLs;
    char **result = malloc(unionSize*sizeof(char *));
   
    for(i = 0; i < unionSize; i++){
        position[i] = i+1; 
        finalP[i] = 0;
    }

    URLs = copySet(unionS); 
    permutation(position, 0, unionSize, finalP, argc, URLs, &totalScaledFootrule, columnSize, rankTable); 
    printf("%.6f\n", totalScaledFootrule);
    for(i = 0; i < unionSize; i++){
        //printf("%d\n",finalP[i]);
        pos = finalP[i];
        for(j = 0; j < pos; j++){
            if((j+1) == pos){
                result[pos-1] = URLs[i]; 
            }
        }
    }

    //print rank in order
    for(i = 0; i < unionSize; i++){
         printf("%s\n",result[i]);
    }
    

    //free memory
   for(i = 0; i < argc-1; i++){
        for(j = 0; j < columnSize[i]; j++){
            free(rankTable[i][j]);
        }
        free(rankTable[i]);
    }

    //free memory
    for(i = 0; i < unionSize; i++){
        free(URLs[i]);
    }

    free(rankTable);
    free(URLs);
    free(result);
    disposeSet(unionS); 
    

    return 1;
}


int getRank(char *filename, char ***Table)
{
	FILE *rankfp;
	char buff[SIZE];
	char *curr;
	int i, count;
	Set seen = newSet();
	Queue q = newQueue();

	//open file 
	if((rankfp = fopen(filename, "r")) == NULL){
    	fprintf(stderr, "CAN'T OPEN FILE: %s\n", filename);
  		exit(1);
    }

    while(fscanf(rankfp, "%s", buff) != EOF){
    	if(strstr(buff, "url") != NULL){
    		if(!isElem(seen, buff)) {
    			insertInto(seen, buff);		// avoid duplicates
    			enterQueue(q, buff);		
    		}
    	}
    }

    fclose(rankfp);
    count = queueSize(q);
    char **column = malloc(sizeof(char *)*count);   //malloc for column to store URL
    //printf("%d\n",queueSize(q));
    
    for(i = 0; i < count; i++){
        curr = leaveQueue(q);   // first in first out
        column[i] = curr;
    	
    }

    *Table = column;
    return (count);
}

//swap position of array
void swap(int *position, int i, int j)
{
    int temp;
    temp = position[i];
    position[i] = position[j];
    position[j] = temp;
    return;
}

//permuatation of P position
void permutation(int *position, int start, int unionSize, int*finalP, int argc, char **URLs, double *totalScaledFootrule, int *columnSize, char ***rankTable)
{

  	double curr; //current scaled footrule
    int j;
    curr = 0.0;
    if (unionSize == start){   
    	curr = calScaledFootrule(unionSize, columnSize, rankTable, URLs, position, argc);
    	if(curr < *totalScaledFootrule){
    		*totalScaledFootrule = curr;
    		for(j = 0; j < unionSize; j++){
    			finalP[j] = position[j];
    		}
    	
    	}
    	return;
    }

    for( j = start; j < unionSize ; j++){
        swap(position, start, j);
        permutation(position, start+1, unionSize, finalP, argc, URLs, totalScaledFootrule, columnSize, rankTable);
        swap(position, start, j);
    }
    return;
}

//calculate scaled footrule
double calScaledFootrule(int unionSize, int *columnSize, char ***rankTable, char **URLs, int *position, int argc)
{
    double total = 0.0,a = 0.0, b = 0.0;
    int i, j, index;

    for(i = 0; i < unionSize; i++){
        for(j = 0; j < argc-1 ; j++){
            index = 0;
            index = getIndex(URLs[i], rankTable[j], columnSize[j]);
            if(index == 0) continue;
            a  = (double)index/(double)columnSize[j];
            b = (double)(position[i])/(double)unionSize;
            total += fabs(a-b);
        }
    }
    //printf("%.6f\n",total);
    return total;
}

//get position of a URL
int getIndex(char *URL, char **column, int size)
{
    int i = 0;
    while(i < size){
        if(strcmp(URL, column[i]) == 0)
            return (i+1);
        i++;
    }
    return 0;
}