#ifndef _SOCKET_H_
#define _SOCKET_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#define LISTENQ 10
#ifndef MAXLINE
#define MAXLINE 4096
#endif

#define BUFFERSIZE 8192
void sig_handle(int signo);
int Createsockfd(struct sockaddr_in *sockaddr);
int Bind(int sockfd, struct sockaddr_in* sockaddr);
int Listen(int sockfd, int listenNum);
int acceptClient(int listenfd, struct sockaddr_in* sockaddr);
int tcp_connect(const char *host);





#endif

