#ifndef _FILEOPERATE_H_
#define _FILEOPERATE_H_
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>
#include "MD5.h"
#define MAXBUF 1024
#ifndef MAXLINE
#define MAXLINE 4096
#endif
int rcv_data(int sockfd,char* filename);
int send_data(int sockfd,char* filename);
ssize_t writen(int fd,const void *buf,size_t nbytes);
ssize_t readn(int fd,void *buf,size_t nbytes);
int sendmd5(int fd,char *filename);
#endif
