/*************************************************************************
	> File Name: Myepoll.c
	> Author: Bely
	> Mail: xiongli_meng@163.com 
	> Created Time: Mon 05 Jan 2015 09:36:21 PM CST
 ************************************************************************/

#include "Myepoll.h"

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock,F_GETFL);
    if(opts<0)
    {
	perror("fcntl(sock,GETFL)");
	return;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
	perror("fcntl(sock,SETFL,opts)");
	return;
    }
}

int changeepollctl(int epfd,int sockfd,uint32_t events,int op)
{
    struct epoll_event event;
    event.data.fd = sockfd;
    event.events = events;
    if(epoll_ctl(epfd, op,sockfd,&event)<0)
    {
 	perror("epoll_ctl failed\n");
        printf("%x \n", op);
        exit(1);
    }
    return 0;
}

int epollIn(int sockfd,struct epoll_event* event,struct sockaddr_in servaddr)
{
   char line[MAXLINE];
   char *head = line;
   int recvNum = 0;
   int count=0;
   int bReadOk = 0;
   memset(line,0,MAXLINE);

   int sin_size=sizeof(struct sockaddr_in);
   recvNum=recvfrom(sockfd,line,MAXLINE,0,(struct sockaddr*)&servaddr,&sin_size);
   line[recvNum] = '\0';
   if(recvNum < 0)
   {
        perror("recvNum\n");
	return 0;
   }
   printf("recvNum:%s \n", line);
   /*while(1)
   {
      //recvNum = read(sockfd,head+count,MAXLINE-1);
      int sin_size=sizeof(struct sockaddr_in);
      recvNum=recvfrom(sockfd,head+count,MAXLINE,0,(struct sockaddr*)&servaddr,&sin_size);
      if(recvNum < 0)
      {
	 if(errno == EAGAIN)  //no data to be dealt with
	 {
	     bReadOk = 1;
	     break;
	 }
		   
	 else if(errno == ECONNRESET)
	 {
	     close(sockfd);
	     (*event).data.fd = -1;
	     printf("counterpart send out RST\n");
	     break;
	 }
		   	
         else if(errno == EINTR)
	 {
	    // the singal is interruptted
	    continue;
         }
	 else
         {
	    printf("recv unrecovable error\n");
	    close(sockfd);
	    (*event).data.fd = -1;
	    break;		   
	 }
		    
      }
                    
     else if(recvNum == 0) // the client has closed 
     {
          close(sockfd);
          (*event).data.fd = -1;
	  printf("counterpart has shut off\n");
          break;
     }
		
     else
     {
         count += recvNum;
         if(recvNum == MAXLINE)
         {
	    continue; // more data need to be read
	 }

	 else
         {
	    bReadOk = 1;
	    break;
	 }
      }
		      

   }
   
   if(bReadOk == 1)
   {
       line[count] = '\0';	   
       printf("%d data from client:%s\n",count,head);
       return 1;
   }*/

   return 1;
}

int epollOut(int sockfd,struct epoll_event* event, char *pstr,struct sockaddr_in servaddr)
{
   char line[MAXLINE];
   memset(line,0,sizeof(line));

   int bWritten = 0;
   int writenlen = 0;
   int count = 0;
   char *head = line;

   memcpy(line,pstr,strlen(pstr));
   while(1)
   {
      writenlen = write(sockfd,head+count,strlen(head+count));
      //writenlen = sendto(sockfd,head+count,strlen(head+count),0,(struct sockaddr*)&servaddr,sizeof(struct sockaddr));
      printf("writenlen :%d,  count : %d \n",writenlen,count);
     // printf("pstr: %s\n", head+count);
      if(writenlen == -1)
      {
			if(errno == EAGAIN)
			{
			    continue;
			}
			else if(errno == ECONNRESET)
			{
			   close(sockfd);
			   (*event).data.fd = -1;
			   printf("counterpart send out RST\n");
			   break;
			}
			else if(errno == EINTR)
		  	{
			   // the singal is interruptted
			   continue;
		   	}
		   	else
		   	{
			   printf("send unrecovable error\n");
			   close(sockfd);
			   (*event).data.fd = -1;
                           break;
		   	}
      }
		
     else if(writenlen == 0)
     {
		   	close(sockfd);
		   	(*event).data.fd = -1;
		   	printf("counterpart has shut off\n");
		   	break;
     }

		  
     else
     {
			count += writenlen;
		   	if(writenlen == MAXLINE)
		   	{
			    continue; // more data need to be read
		   	}

		   	else
		   	{
			    bWritten = 1;
			    break;
		   	}
      }
   }
		

   if(bWritten == 1)
   {
	//printf("send to client:%s\n",head);
	return 1;
   }
   return 0;
}


		
