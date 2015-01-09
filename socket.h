#ifndef SOCKET_H
#define SOCKET_H

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

int initsocket(sockaddr_in &sockaddr, int nPort);
int Connect(sockaddr_in &sockaddr);
int Bind(int sockfd, sockaddr_in sockaddr);
int Listen(int sockfd, int listenNum);
int Accept(int listenfd, sockaddr_in sockaddr);
int Connect(int serverfd, sockaddr_in serveraddr);






#endif

