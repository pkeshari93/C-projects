/*****************************************************************************
 * Name : Prakhar Keshari
 * Username : pkeshari
 * Course : CS3411 Fall 2017
 * Description : Program 3, header file for recordio.c
 ******************************************************************************
 */

#ifndef INDEXER_H
#define INDEXER_H


typedef struct
{
	int position;
	int length;
} record_descriptor;

int rio_open(const char*pathname, int flags, mode_t mode);

int rio_lseek(int fd, int offset, int whence);

void * rio_read(int fd, int* return_value);

int rio_write(int fd, const void *buf, int count);

int rio_close(int fd);


#endif
