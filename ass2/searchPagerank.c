//searchPagerank.c
//written by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)
//Given search terms (words) as commandline arguments
//Finds pages with all search terms and outputs (to stdout) top ten pages in descending order of PageRank

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "set.h"
#include "normalise.h"
#include "queue.h"

#define SIZE 50

//main function 
int main(int argc, char *argv[]){
    if(argc < 2){  //makes sure there is atleast one keyword
  		fprintf(stderr, "Usage: %s [search terms]\n", argv[0]);
  		exit(1);
    }

    int i,k, count, found;       //counters
    char buff[SIZE];       
    char *str, *keyword, *URL, *curr;
    Queue all = newQueue();       //Queue to store all url that keywords exist
    Set unionSet = newSet();      //Set of all url that keywords exist

    Set s[argc-1]; 			  //an array of sets for each keyword
    FILE *fp, *pl;        //file pointers

    if((fp = fopen("invertedIndex.txt", "r")) == NULL){               //open file
		  fprintf(stderr,  "Error: Cannot open invertedIndex.txt \n");
    	exit(1);
    }
    
    
    k = 0;
    found = 0;

    for(i = 1; i < argc; i++){
  		while(fgets(buff, SIZE, fp) != NULL){       //get line in file
  		   	str = strstr(buff,"url");               // take only URLs & ignore keywords
  		   	//printf("%s\n",str );
  		   	keyword = strtok(buff, " \n");          //get keyword
  		   	//printf("%s\n",keyword);
  	   		if(strcmp(normalise(argv[i]), keyword) == 0){ //check if user input is a valid keyword
    			s[k] = newSet();		
    			URL = strtok(str, " \n");
            	found ++;
    			while(URL != NULL){
    				insertInto(s[k], normalise(URL));
				    if(!isElem(unionSet, normalise(URL))){ 		//chech URL existence in set
				          	insertInto(unionSet, normalise(URL));	//enter into Set for checking
				           	enterQueue(all, normalise(URL));		//store all URL for all keywords
				    }
    				//showSet(s[k]);
    				URL = strtok(NULL, " \n");	
    			}
  			}
	   	}
	   	rewind(fp);
	   	k++; 		
   	}
    fclose(fp);

    if(found < argc-1){
      return 0;
    }
   	
    Set match = newSet();         //set of all URL which all keywords exist

    if(argc < 3){				//no URL contain all search terms
      match = s[0];
    }
  
    for(i = 0; i < nElems(unionSet) ; i++){
    	curr = leaveQueue(all);			//get url from queue
    	count = 0;
  		for(k = 0; k < argc-1; k++){
  			if(isElem(s[k], curr)){
  				count++;				//count number of existence
  			}
  		}

  		if(count == argc-1){			//if count equal to number of keywords, URL contains all keywords
  			insertInto(match, curr);	//sSet of URL that contains all keywords
  		}
    }
    

    //showSet(new);

    if((pl = fopen("pagerankList.txt", "r")) == NULL){
      fprintf(stderr,  "Error: Cannot open pagerankList.txt \n");
      exit(1);
    }
    
    count = 0; 

    while(fgets(buff, SIZE, pl) != NULL){
      URL = strtok(buff," ,\n");
      //printf("%s\n",URL );
      if(isElem(match, normalise(URL)) && count <= 10){
        //print to stdout
        printf("%s\n",URL);
        count ++;
      }
    }
    
    fclose(pl);
    disposeSet(match);
    return 1;
}


