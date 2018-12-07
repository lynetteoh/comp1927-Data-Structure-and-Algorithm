CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2 -pg
LIBS=graph.o queue.o set.o mergeSort.o readData.o BSTree.o normalise.o

all: 	inverted.o pagerank.o searchPagerank.o searchTfIdf.o scaledFootrule.o $(LIBS)
	$(CC) $(CFLAGS) -lm -o pagerank pagerank.o $(LIBS) 
	$(CC) $(CFLAGS) -o inverted inverted.o $(LIBS) 
	$(CC) $(CFLAGS) -o searchPagerank searchPagerank.o $(LIBS) 
	$(CC) $(CFLAGS) -lm -o searchTfIdf searchTfIdf.o $(LIBS) 
	$(CC) $(CFLAGS) -o scaledFootrule scaledFootrule.o $(LIBS)
	mkdir -p bin/ && mv *.o bin/
	
searchTfIdf.o : searchTfIdf.c
searchPagerank.o : searchPagerank.c
inverted.o : inverted.c
pagerank.o : pagerank.c
BSTree.o : BSTree.c BSTree.h
set.o : set.c set.h
queue.o : queue.c queue.h
graph.o : graph.c graph.h
readData.o: readData.c readData.h
normalise.o: normalise.c normalise.h
scaledFootrule.o : scaledFootrule.c	

clean :
	rm -f pagerank inverted searchPagerank scaledFootrule searchTfIdf *.o core
