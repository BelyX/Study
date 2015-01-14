/*************************************************************************
	> File Name: test.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Tue 06 Jan 2015 07:20:09 PM CST
 ************************************************************************/

#include<stdio.h>

int main()
{
	FILE* fp;
	fp = fopen("1.txt","w+");
	int i = 0;
	for(i = 0; i<1024*1024*1024; i++)
	{
		fputc('1',fp);
	}
}
