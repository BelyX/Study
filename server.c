/*************************************************************************
    > File Name: TCP_server.c
    > Author: xiongli
    > Mail: xiongli_meng@163.com 
    > Created Time: Sun 21 Dec 2014 01:49:28 AM PST
 ************************************************************************/

#include "Myepoll.h"
#include "socket.h"
#include "fileoperate.h"
#define SIZE_SEND 800


int main(int argc, char **argv)
{
    int efd;
    int nfds;
    int i;
    int n;
    int listenfd;
    int clientfd;
    int sockfd;
    int udpfd;
    int nCount = 0;
    char line[MAXLINE];
    struct epoll_event event;
    struct epoll_event events[64];
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    FILE *fp;


    efd = epoll_create(256); 
    printf("%d\n",efd);    

    //建立TCP连接
    initsocket(&servaddr,argc,argv);
    listenfd = Createsockfd(AF_INET,SOCK_STREAM);
    Bind(listenfd,&servaddr);
    Listen(listenfd,5);

    setnonblocking(listenfd);
    changeepollctl(efd,listenfd,EPOLLIN|EPOLLET,EPOLL_CTL_ADD);

    for( ; ;)
    {
	nfds = epoll_wait(efd,events,64,-1);

	for(i = 0; i<nfds; ++i)
	{
	    if(events[i].data.fd == listenfd)
	    {
		clientfd = acceptClient(listenfd,&clientaddr);
		setnonblocking(clientfd);
                udpfd = Createsockfd(PF_INET,SOCK_DGRAM);


                if((fp = openfile("1.txt","r+")) == NULL)
                {
                    continue;
                }
                changeepollctl(efd,clientfd,EPOLLIN | EPOLLET | EPOLLOUT,EPOLL_CTL_ADD);
	    }

	    else if(events[i].events & EPOLLIN)
	    {
		if((sockfd = events[i].data.fd) <0)
		{
		    continue;
		}
		
		if(epollIn(udpfd,&events[i],servaddr) == 1)
                {
                    changeepollctl(efd,sockfd,EPOLLOUT | EPOLLET,EPOLL_CTL_MOD);
                }
		
            }

	    else if(events[i].events & EPOLLOUT)
	    {
		sockfd = events[i].data.fd;
                char pstr[1024];
                int nCount = 0;
                long int filesize = Getfilesize("1.txt");
                sprintf(pstr,"%ld",filesize);
                printf("filesize:%ld\n",filesize);
                pstr[strlen(pstr)] = '\0';
                nCount = write(sockfd,pstr,strlen(pstr));
                printf("writenlen :%d\n",nCount);
                
		//while(!feof(fp))
		{
                   nCount = fread(pstr,1,SIZE_SEND,fp);
                   if(nCount==0)
                   {
                     perror("fail to read file\n");
                   }
		   pstr[nCount] = '\0';
                   if(epollOut(sockfd,&events[i],pstr,servaddr) == 1)
                   {
                      continue;
                   }
		} 
                fclose(fp);
                changeepollctl(efd,sockfd,EPOLLIN|EPOLLET,EPOLL_CTL_MOD);
	    }
	}
    }

   return 0;
}
