/*
  client to test listIteratorInt.
  Written by ....
*/
/*
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listIteratorInt.h"
#define MAX 100

int getCommand(char *buf);

int main(int argc, char *argv[])
{	
	IteratorInt list;
	int v=0;
	int done;
	int temp1;
	int *temp;
	char fname[MAX]; 
	char cmd[MAX];
	FILE *f;
	FILE *out;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s FileName\n", argv[0]);
		return EXIT_FAILURE;
	}
	strcpy(fname,argv[1]);
	if ((f = fopen(fname,"r")) == NULL) {
		fprintf(stderr, "Can't open file %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	list = getIteratorInt(f);
	fclose(f);
	done = 0;
	while (!done && getCommand(cmd)) {
		switch (cmd[0]) {
		case 's':
			// move to next line
			showIteratorInt(stdout,list);
			break;
		case 'n':
			// move to next line
			hasNext(list);
			temp = next(list);
			printf("value : %d\n", *temp);
			break;
		case 'p':
			// move to previous line
			hasPrevious(list);
			temp = previous(list);
			printf("value : %d\n", *temp);
			break;
		case 'a':
			scanf("%d",&v);
			printf("\n\n");
			add(list,v);
			out = fopen("out", "w");
			fprintf(out,"%d",v);
			showIteratorInt(stdout,list);
			break;
		case 'd':
			// delete current line
			temp1 = delete(list);
			printf ("%d\n\n",temp1);

			break;
		case 'r':
			// delete current line
			reset(list);
			showIteratorInt(stdout,list);
			break;
		case 'h':
			// move to next line
			printf("insert num to find: \n");
			scanf("%d",&v);
			findNext(list,v);
			break;
		case 'g':
			// move to next line
			printf("insert num to find: \n");
			scanf("%d",&v);
			findPrevious(list,v);
			break;	
		case 'b':
			// move to next line
			printf("insert num: \n");
			scanf("%d",&v);
			set(list,v);
			showIteratorInt(stdout,list);
			break;	

		case 'q':
			done = 1;
			break;
		}
	}	
	return EXIT_SUCCESS;
}

int getCommand(char *buf)
{
	printf("> ");
	return (fgets(buf, MAX, stdin) != NULL);
}
*/