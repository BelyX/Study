#include "fileoperate.h"

FILE* openfile(char* filename, char* opentype)
{
    FILE* fp = NULL;
    if(NULL == filename || NULL == opentype)
	{
	    printf("invalid file\n");
		return NULL;
	}
	
	fp = fopen(filename,opentype);
	if(fp == NULL)
	{
	   perror("file fail to open\n");
	   return NULL;
	}
	return fp;
}
