#ifndef _FILEOPERATE_H_
#define _FILEOPERATE_H_
#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
FILE* openfile(char* filename, char* opentype);
long int Getfilesize(const char* filename);
#endif
