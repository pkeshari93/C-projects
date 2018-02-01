/*****************************************************************************
 * Name : Prakhar Keshari
 * Username : pkeshari
 * Course : CS3411 Fall 2017
 * Description : Program 3, header file for indexer
 ******************************************************************************
 */

#ifndef INDEXER_H
#define INDEXER_H

//structure for record descriptor 
typedef struct
{
	int position;
	int length;
} record_descriptor;

// char* concat(const char *s1, const char *s2);
void read_char(int fd_in, int fd_out);

#endif
