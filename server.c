/*************************************************************************
    > File Name: TCP_server.c
    > Author: xiongli
    > Mail: xiongli_meng@163.com 
    > Created Time: Sun 21 Dec 2014 01:49:28 AM PST
 ************************************************************************/

#include "Myepoll.h"
#include "fileoperate.h"
#include "socket.h"
#include "MD5.h"
#define FILENAME "1.txt"


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

    initsocket(&servaddr,argc,argv);
    listenfd = Createsockfd(AF_INET,SOCK_STREAM);
    Bind(listenfd,&servaddr);
    Listen(listenfd,5);

    efd = epoll_create(256); 
    printf("%d\n",efd);
    setnonblocking(listenfd);

    changeepollctl(efd,listenfd,EPOLLIN | EPOLLET,EPOLL_CTL_ADD);

    for( ; ;)
    {
	nfds = epoll_wait(efd,events,64,-1);

	for(i = 0; i<nfds; ++i)
	{
	    if(events[i].data.fd == listenfd)
	    {
		clientfd = acceptClient(listenfd,&clientaddr);
		setnonblocking(clientfd);
                changeepollctl(efd,clientfd,EPOLLIN | EPOLLET | EPOLLOUT,EPOLL_CTL_ADD);
       
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
                    changeepollctl(efd,sockfd,EPOLLOUT | EPOLLET,EPOLL_CTL_MOD);
                }
		
            }

	    else if(events[i].events & EPOLLOUT)
	    {
		sockfd = events[i].data.fd;
                char pstr[1024];
                int nCount = 0;
                long int fsize = Getfilesize(FILENAME);
                sprintf(pstr,"%ld",fsize);
                printf("filesize:%ld\n",fsize);
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
		char md5_sum[MD5_LEN + 1];
                if(!CalcFileMD5(FILENAME, md5_sum))
       		{
       			puts("Error occured!");
      			return NULL;
  		 }
  
  		printf("Success! MD5 sum is :%s \n", md5_sum);
                nCount = write(sockfd,md5_sum,strlen(md5_sum));
                printf(" md5_sum send :%s\n",md5_sum);
                printf("writenlen :%d\n",nCount);
                changeepollctl(efd,sockfd,EPOLLIN | EPOLLET,EPOLL_CTL_MOD);
	    }
	}
    }

   return 0;
}
