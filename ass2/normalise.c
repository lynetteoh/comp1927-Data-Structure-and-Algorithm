//normalise.c
//written by Lean Lynn Oh (z5144464) & Chun Lam Lum (z5144324)
//use to remove spaces and punctuation
//reference: How Do I Trim Leading/Trailing Whitespace in Standard Way from https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "normalise.h"

char *normalise(char *);


char *normalise(char *string) 
{
    int i;
    char *end;

    for(i = 0; string[i] != '\0'; i++){
        string[i] = tolower(string[i]);  //lowercase the string
    } 

    if (*string == 0){
        return string;
    }
    //remove leading spaces and punctuation
    while ((*string && (isalnum(*string)== 0)) || (*string && (isspace(*string) != 0))){
       string++;
    }
	
    //remove trailing spaces and punctuation
    end = string + strlen(string) - 1;  
    while ((end >= string && (isalnum(*end) == 0)) || (end >= string && (isspace(*end) != 0))){
	   end--;
    }
    *(end + 1) = '\0'; //add new null terminator
  

    return string;
}
