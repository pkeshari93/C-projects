/*****************************************************************************
 * Name : Prakhar Keshari
 * Username : pkeshari
 * Course : CS3411 Fall 2017
 * Description : Program 3, This is a library of routines that performs file I/O
 ******************************************************************************
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "recordio.h"

int rio_fd; //packed file descriptor returned by rio_open
int offset_location; //resulting offset 
char *inx = ".rinx.";
struct stat buf;
char bufcheck[256];
int fd1 = 0, fd2 = 0;
record_descriptor r;

/*
 *Function : Takes in 2 strings and combines them
 *Return   : result : name of indexer file following the convention .rinx<datafile>
 *Args	   : *s1 ".rinx"
 		   : *s2 "datafile name"
*/
char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/*
 *Function : Takes in filename to be opened with correspoding flags and modes
 *Return   : rio_fd : an int containing 2 file descriptors packed into one
 *Args	   : char* pathname : filename or path of file
 		   : flags : O_flags
 		   : mode  : file permissions
*/
int rio_open(const char*pathname, int flags, mode_t mode){

	//creating pathname for indexer file
	inx = concat(".rinx.", pathname);

	// printf("index filename %s\n", inx);

	//check if index file is present if O_RDONLY or O_RDWR is requested, if not report error
	if((flags == 0) | (flags == 1) | (flags == 2)){
		if(stat(inx, &buf) < 0){
			perror("index file doesn't exist\n");
			exit(-1);
		}
	}

	fd1 = open(pathname, flags, mode);
	fd2 = open (inx, flags, mode);
	rio_fd = (fd1 << 16) | fd2;

	return rio_fd;
}

/*
 *Function : lseek like routine, seeks both datafile and indexer to the beginneing 
 			of a record to perform next I/O
 *Return   : n_d: offset bytes in datafile
 *Args	   : fd : packed fd containing datafile and indexer's file descriptors 
 		   : offset : same as lseek
 		   : whence : same as lseek
*/
int rio_lseek(int fd, int offset, int whence){


	int byte_offset = sizeof(record_descriptor) * offset;
	
	//sotres the resulting offset in datafile
	int n_i = 0;
	int n_d = 0;

	fd1 = (fd >> 16) & 0xffff; //datafile fd
	fd2 = fd & 0xffff; //indexer fd

	n_i = lseek(fd2, byte_offset, whence);
	
	if(read(fd2, &r, sizeof(record_descriptor)) < 0){
		perror("reading error");
		exit(-1);
	}
	
	n_d = lseek(fd1, r.position, SEEK_SET);

	return n_i;
}

/*
 *Function : implementation of open like routine
 *Return   : buf : a pinter to a buffer that will contain the desried record
 *Args	   : fd : packed fd containing datafile and indexer's file descriptors 
 		   : return_value : 
*/
void * rio_read(int fd, int* return_value){


	fd1 = (fd >> 16) & 0xffff;
	fd2 = fd & 0xffff;

	// printf("fd1 = %d || fd2 = %d\n\n", fd1, fd2);

	int length = r.length;

	// char* p = NULL;
	char* buf = NULL;
	buf = malloc(length);
	* return_value = length;



	if(read(fd1, buf, length) < 0){
		perror("read error");
		exit(-1);
	}

	// p = buf;
	// printf("%d----------\n", length);

	return buf;
}

/*
 *Function : implementation of write like routine
 *Return   : number of bytes written 
 *Args	   : fd : packed fd containing datafile and indexer's file descriptors 
 		   : buf : string to be written 
 		   : number of bytes to be written 
*/
int rio_write(int fd, const void *buf, int count){

	int new_pos = r.position + r.length;

	fd1 = (fd >> 16) & 0xffff;
	fd2 = fd & 0xffff;

	record_descriptor r1;

	if(read(fd2, &r, sizeof(record_descriptor)) < 0){
		perror("read error");
		exit(-1);
	}

	r1.length = strlen(buf);
	r1.position = new_pos;



	lseek(fd2, 0, SEEK_END);
	lseek(fd1, 0, SEEK_END);

	write(fd2, &r1, sizeof(record_descriptor));
	write(fd1, buf, strlen(buf));

	lseek(fd2, 0, SEEK_SET);
	lseek(fd1, 0, SEEK_SET);

	

	return strlen(buf);
}

/*
 *Function : close like routine that close both indexer and data file once 
 			operations like open, lseek, etc. are complete
 *Return   : returns 0 when successful. -1 otherwise
 *Args	   : fd : packed fd containing datafile and indexer's file descriptors  
*/
int rio_close(int fd){

	// int fd1, fd2;

	fd1 = (fd >> 16) & 0xffff;
	fd2 = fd & 0xffff;

	if(close(fd1) < 0){
		perror("error close datafile");
		exit(-1);
	}

	if(close(fd2) < 0){
		perror("error close index file");
		exit(-1);
	}

	return 0;
}



