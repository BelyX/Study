#ifndef _SOCKET_H_
#define _SOCKET_H_

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define SERVER_PORT 9000

int initsocket(struct sockaddr_in *sockaddr, int argc, char** argv);
int Createsockfd(int af,int type);
int Bind(int sockfd, struct sockaddr_in* sockaddr);
int Listen(int sockfd, int listenNum);
int acceptClient(int listenfd, struct sockaddr_in* sockaddr);
int connectClient(int serverfd, struct sockaddr_in* serveraddr);






#endif

