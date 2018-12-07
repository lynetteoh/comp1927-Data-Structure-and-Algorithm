// searchTfIdf.c
//Written by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)
//Content-based search engine that uses tf-idf values of all query terms for ranking
//Calculate tf-idf values for each query term in a page, and rank pages based on the summation of tf-idf values for all query terms

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "queue.h"
#include "set.h"
#include "normalise.h"
#include "readData.h"

#define SIZE 50

typedef struct sum *Sum;
typedef struct sum{
    char *url;  //url name
    double value;  // term frequency in a document
}sum;

/*function headers*/
int countDocumentNumber();
double calculateTfIdf(char **, int, char *, double *);
Set searchKeyword(int, char **);
double inverseDocFrequency(int, char *);


int main(int argc, char *argv[])
{
	Set match = newSet();
    Queue document = newQueue();
    FILE *collection;
    int i, j, urlNo, d; 
    double idf[argc-1];
    char buff[SIZE];
    char filename[SIZE];

    if(argc < 2){
		fprintf(stderr, "Usage: %s [keyword]\n", argv[0]);
		exit(1);
    }

    

    match = searchKeyword(argc, argv);
    //showSet(match);
    if(emptySet(match)){			//no URL contain all search terms 
        return 0;
    }

    d = countDocumentNumber(); //total number of documents

    for(i = 0; i < argc-1; i++){
        idf[i] = inverseDocFrequency(d, argv[i+1]);
        //printf("%f\n",idf[i]);
    }

    if((collection = fopen("collection.txt", "r")) == NULL){
        fprintf(stderr,  "Error: Cannot open collection.txt \n");
        exit(1);
    }

    while(fscanf(collection, "%s", buff) != EOF){
        if(isElem(match, buff)){        //ignore duplicate links
            enterQueue(document, buff);
        }
    }

    fclose(collection);

    urlNo = queueSize(document);
    struct sum *tfIdf = malloc(urlNo* sizeof(sum));
    struct sum temp;

    for(i = 0; i < urlNo ; i++){
        tfIdf[i].url = leaveQueue(document);
        strcpy(filename, tfIdf[i].url);
        strcat(filename,".txt");
        tfIdf[i].value = calculateTfIdf(argv, argc, filename, idf);
    }

    //bubble sort
    for (i = 0; i < urlNo -1; i++){
     	for (j = 0; j < urlNo-i-1; j++) 
           if (tfIdf[j].value < tfIdf[j+1].value){
	           	temp = tfIdf[j];
	           	tfIdf[j] = tfIdf[j+1];
	           	tfIdf[j+1] = temp;
           }

     }
       

    for(i = 0; i < urlNo; i++){
        printf("%s   %.6f\n",tfIdf[i].url, tfIdf[i].value);
    }
    return 1;
}


//get total number of documents
int countDocumentNumber()
{
	//char buff[SIZE];
	//FILE *collection;
	//Set seen = newSet();
   

	/*if((collection = fopen("collection.txt", "r")) == NULL){
        fprintf(stderr,  "Error: Cannot open collection.txt \n");
        exit(1);
    }
    
    while(fscanf(collection, "%s", buff) != EOF){
    	if (!isElem(seen, buff)){ //ignore duplicate links
            enterQueue(q, buff);
            insertInto(seen, buff);
        }
    }*/


    int total; // total number of documents
    Queue q = newQueue();
    

    getCollection(q);
    total = queueSize(q);
    return total;
}

//calculate tfIdf values for each URL
double calculateTfIdf(char **term, int argc, char *filename, double *idf)
{
    char buff[SIZE];
    double TfIdf;
    int i, count;
    FILE *fp;

    TfIdf = 0.0;

    if ((fp = fopen(filename, "r")) == NULL){
        fprintf(stderr, "Can't open file %s\n", filename);
        exit(1);
    }
    
    for(i = 0; i < argc-1; i++){
        count = 0;

        while(fscanf(fp, "%s", buff) != EOF){
            if(strcmp(buff, "Section-2") == 0){  //check for given keyword
                fscanf(fp,"%s",buff);
                break;
            }
        }

        while(strcmp(buff,"#end") != 0){
            if(strcmp(normalise(buff), term[i+1]) == 0){
                count ++; 
            }
            fscanf(fp,"%s",buff);
        }
        TfIdf += (count * idf[i]);
        rewind(fp);
    }
    
    fclose(fp);       
    return TfIdf; 
}


// calculate inverse document frequency 
// logarithm of total number of documents divided by documents containing term
double inverseDocFrequency(int totalDocument, char *term)
{
    int termD;
    double idf;
    FILE *inverted;
    char *URL, *keyword, *str;
    char buff[SIZE];
    Set s = newSet();

    if((inverted = fopen("invertedIndex.txt", "r")) == NULL){
        fprintf(stderr,  "Error: Cannot open invertedIndex.txt \n");
        exit(1);
    }

    while(fgets(buff, SIZE, inverted) != NULL){
            str = strstr(buff,"url");
            keyword = strtok(buff, " \n");
            if(strcmp(normalise(term), keyword) == 0){ //check if user input is a valid keyword
                URL = strtok(str, " \n");
                while(URL != NULL){
                    insertInto(s, normalise(URL));
                    URL = strtok(NULL, " \n");    
                }
            }
        }

    fclose(inverted);
    termD = nElems(s);
    idf = log10(totalDocument/(double)termD);
    return idf;
}


//get URLs that contain all search keywords
Set searchKeyword(int argc, char **term){
    char *URL, *keyword, *str, *curr;
    char buff[SIZE];
    Set s[argc-1];
    FILE *inverted;
    int k, i, found, count;
    Set unionSet = newSet();
    Queue all = newQueue();

    if((inverted = fopen("invertedIndex.txt", "r")) == NULL){
        fprintf(stderr,  "Error: Cannot open invertedIndex.txt \n");
        exit(1);
    }
    
    k = 0;
    found = 0;    
    for(i = 1; i < argc; i++){
        while(fgets(buff, SIZE, inverted) != NULL){
            str = strstr(buff,"url");
            keyword = strtok(buff, " \n");
            if(strcmp(normalise(term[i]), keyword) == 0){       //check if user input is a valid keyword
                s[k] = newSet();        
                URL = strtok(str, " \n");
                found ++;
                while(URL != NULL){
                    insertInto(s[k], normalise(URL));
                    if(!isElem(unionSet, normalise(URL))){      //chech URL existence in set
                        insertInto(unionSet, normalise(URL));   //enter into Set for checking
                        enterQueue(all, normalise(URL));        //store all URL for all keywords
                    }
                    //showSet(s[k]);
                    URL = strtok(NULL, " \n");  
                }
            }
        }
        rewind(inverted);
        k++;        
    }
    fclose(inverted);


    Set match = newSet();

    if(found < argc-1){
      return match;
    }
 

    if(argc < 3){
      match = s[0];
    }
  
    for(i = 0; i < nElems(unionSet) ; i++){
        curr = leaveQueue(all);         //get url from queue
        count = 0;
        for(k = 0; k < argc-1; k++){
            if(isElem(s[k], curr)){
                count++;                //cpunt number of existence
            }
        }

        if(count == argc-1){            //if count equal to number of keywords, URL contains all keywords
            insertInto(match, curr);    
        }
    }
    
    
    //showSet(match);
    return match;
}

