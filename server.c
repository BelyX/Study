/*************************************************************************
    > File Name: TCP_server.c
    > Author: xiongli
    > Mail: xiongli_meng@163.com 
    > Created Time: Sun 21 Dec 2014 01:49:28 AM PST
 ************************************************************************/

#include "Myepoll.h"
#include <sys/stat.h>
#include<unistd.h>
#include "socket.h"
#include "fileoperate.h"


int main(int argc, char **argv)
{
    int efd;
    int nfds;
    int i;
    int n;
    int listenfd;
    int clientfd;
    int sockfd;
    char line[MAXLINE];
    struct epoll_event event;
    struct epoll_event events[64];
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    FILE *fp;
    struct stat fileInfo;


    efd = epoll_create(256); 
    printf("%d\n",efd);    

    initsocket(&servaddr,argc,argv);
    listenfd = Createsockfd(AF_INET,SOCK_STREAM);
    Bind(listenfd,&servaddr);
    Listen(listenfd,5);

    
    setnonblocking(listenfd);
    event.data.fd = listenfd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(efd, EPOLL_CTL_ADD,listenfd,&event)<0)
    {
 	perror("epoll_ctl failed\n");
        exit(1);
    }

    for( ; ;)
    {
	nfds = epoll_wait(efd,events,64,-1);

	for(i = 0; i<nfds; ++i)
	{
	    if(events[i].data.fd == listenfd)
	    {
		clientfd = acceptClient(listenfd,&clientaddr);
		setnonblocking(clientfd);
                
        	/*fp = fopen("1.txt","r+");
		if(fp == NULL)
        	{
	    	   perror("file fail to open\n");
            	   continue;
        	}  */

                if((fp = openfile("1.txt","r+")) == NULL)
                {
                    continue;
                }
                event.data.fd = clientfd;
                event.events = EPOLLIN | EPOLLET | EPOLLOUT;
                epoll_ctl(efd,EPOLL_CTL_ADD,clientfd,&event);
	    }

	    else if(events[i].events & EPOLLIN)
	    {
		if((sockfd = events[i].data.fd) <0)
		{
		    continue;
		}
		
		if(epollIn(sockfd,&events[i]) == 1)
                {
                    event.data.fd = sockfd;
                    event.events = EPOLLOUT | EPOLLET;
                    epoll_ctl(efd,EPOLL_CTL_MOD,sockfd,&event);
                }
		
            }

	    else if(events[i].events & EPOLLOUT)
	    {
		sockfd = events[i].data.fd;
                char pstr[1024];
                int nCount = 0;
                stat("1.txt",&fileInfo);
                sprintf(pstr,"%ld",fileInfo.st_size);
                printf("filesize:%ld\n",fileInfo.st_size);
                pstr[strlen(pstr)] = '\0';
                nCount = write(sockfd,pstr,strlen(pstr));
                printf("writenlen :%d\n",nCount);
                
		while(!feof(fp))
		{
                   nCount = fread(pstr,1,100,fp);
                   if(nCount==0)
                   {
                     perror("fail to read file\n");
                   }
		   pstr[nCount] = '\0';
                   if(epollOut(sockfd,&events[i],pstr) == 1)
                   {
                      continue;
                   }
		} 
                fclose(fp);
                event.data.fd = sockfd;
                event.events = EPOLLIN|EPOLLET;
	        epoll_ctl(efd,EPOLL_CTL_MOD,sockfd,&event); 
	    }
	}
    }

   return 0;
}
