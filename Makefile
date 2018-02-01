#Makefile for program 3
#Author : Prakhar Keshari (pkeshari)
all : indexer testio
indexer : indexer.o
	gcc indexer.o -o indexer
testio : testio.o 
	gcc testio.o -o testio 
recordio : recordio.o 
	gcc recordio.o -o recordio recordio.h
indexer.o : indexer.c indexer.h
	gcc -std=c99 -pedantic -Wall -c indexer.c
testio.o : testio.c 
	gcc -std=c99 -pedantic -Wall -c testio.c
clean :
	rm indexer.o testio.o