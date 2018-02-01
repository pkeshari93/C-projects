/*****************************************************************************
 * Name : Prakhar Keshari
 * Username : pkeshari
 * Course : CS3411 Fall 2017
 * Description : Program 3, This is a stand alone tool called indexer 
 				that accepts a data
 				file as an argument and creates an index file for it and 
 				creates and writes records information, position and length
 				to it
 ******************************************************************************
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "indexer.h"

int fd_input_file, fd_indexer;
char *filename="filename";
char *indexer_name = ".rinx.";
int pos = 0;
record_descriptor r = {0, 0};
record_descriptor r1 = {0, 0};
int bytes = 0;


/*
 *Function : Records length and position into record_descriptor
 			 and writes to .rinx<datafile>
 *Return   : void
 *Args	   : fd1: datafile
 		   : fd2: indexer file
*/ 		   
void read_char(int fd1, int fd2){

	int i = 0;
	char ch = '\0';
	
	//reads chars until end of file is reached
	while (read(fd1, &ch, 1) != 0){ 
		bytes++; //tracks total number of bytes read to calculate position

		//records position
		if(i == 0){
			r.position = bytes-1;
		}
		
		//writes struct to .rinx<datafile> for every record
		if(ch == '\n'){

			pos = pos + bytes;
			
			r.length = i+1;

			if(write(fd2, &r, sizeof(record_descriptor)) < 0){
				perror("write error while writing struct to indexer file");
				exit(-1);
			}

			i = 0;
			continue;
		}
		i++;
	}
	close(fd1);
	close(fd2);
}

/*
 *Function : Takes in 2 strings and combines them
 *Return   : result : name of indexer file following the convention .rinx<datafile>
 *Args	   : *s1 ".rinx"
 		   : *s2 "datafile name"
*/ 	
char* concat(const char *s1, const char *s2 ){
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/*
 *Function : This is the main function of indexer.c
 *Return   : 0 once the control reaches the end
 *Args	   : takes in a datafile
*/
int main(int argc, char** argv){

	//opening datafile
	fd_input_file = open(argv[1], O_RDWR);
	
	filename = argv[1];
	indexer_name = concat(".rinx.", filename);
	
	fd_indexer = open(indexer_name, O_CREAT | O_RDWR, 0600);
	read_char(fd_input_file, fd_indexer);

	close(fd_indexer);
	close(fd_input_file);



	return 0;
}

