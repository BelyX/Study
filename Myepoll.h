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

void setnonblocking(int sock);
int changeepollctl(int epfd,int sockfd,uint32_t events,int op);
int epollOut(int sockfd,struct epoll_event* event, char *pstr,struct sockaddr_in servaddr);
int epollIn(int sockfd,struct epoll_event* event,struct sockaddr_in servaddr);
#endif
