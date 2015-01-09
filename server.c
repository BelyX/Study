/*************************************************************************
    > File Name: TCP_server.c
    > Author: xiongli
    > Mail: xiongli_meng@163.com 
    > Created Time: Sun 21 Dec 2014 01:49:28 AM PST
 ************************************************************************/

#include "Myepoll.h"
#include <sys/stat.h>
#include<unistd.h>


int main(int argc, char **argv)
{
    int efd;
    int nfds;
    int i;
    int n;
    int listenfd;
    int clientfd;
    int sockfd;
    int nPort=SERVER_PORT;
    char line[MAXLINE];
    struct epoll_event event;
    struct epoll_event events[64];
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    FILE *fp;
    struct stat fileInfo;

    if(argc>1)
    {
	nPort = atoi(argv[1]);
    }

    bzero(&servaddr,sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(nPort);

    efd = epoll_create(256); 
    printf("%d\n",efd);
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    

    if(listenfd<0)
    {
	perror("create socket failed!\n");
	exit(1);
    }
    
    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr)) == -1)
    {
	perror("server bind failed\n");
	exit(1);
    }

    if(listen(listenfd,5) == -1)
    {
	perror("server listen failed\n");
	exit(1);
    }

    setnonblocking(listenfd);
    event.data.fd = listenfd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(efd, EPOLL_CTL_ADD,listenfd,&event)<0)
    {
 	perror("epoll_ctl failed\n");
        exit(1);
    }
    printf("server listen succeed\n");

    int nlength = sizeof(clientaddr);
    for( ; ;)
    {
	nfds = epoll_wait(efd,events,64,-1);

	for(i = 0; i<nfds; ++i)
	{
	    if(events[i].data.fd == listenfd)
	    {
		clientfd = accept(listenfd, (struct sockaddr*) &clientaddr,&nlength);
		setnonblocking(clientfd);
		
		if(clientfd <0)
		{
		    perror("connfd<0");
		    exit(1);
		}
                
		char *str = inet_ntoa(clientaddr.sin_addr);
		printf("connect from: %s\n", str);
		event.data.fd = clientfd;
		event.events = EPOLLIN | EPOLLET | EPOLLOUT;
		epoll_ctl(efd,EPOLL_CTL_ADD,clientfd,&event);
       
        	fp = fopen("1.txt","r+");
		if(fp == NULL)
        	{
	    	   perror("file fail to open\n");
            	   continue;
        	}   

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
