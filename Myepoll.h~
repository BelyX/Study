/*************************************************************************
	> File Name: Myepoll.h
	> Author: Bely
	> Mail: xiongli_meng@163.com 
	> Created Time: Mon 05 Jan 2015 09:35:09 PM CST
 ************************************************************************/

#ifndef _MYEPOLL_H_
#define _MYEPOLL_H_
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXLINE 1024
#define SERVER_PORT 9000
void setnonblocking(int sock);
int epollOut(int sockfd,struct epoll_event* event, char *pstr);
int epollIn(int sockfd,struct epoll_event* event);
#endif
