#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dbtime.h"

#define SERVER_PORT 9000
int main(int argc, char **argv)
{
  struct sockaddr_in servaddr;

  int clientfd;
  char serverip[50] = "127.0.0.1";
  
  int nPort=SERVER_PORT;
  if(argc>1)
  {
     nPort = atoi(argv[1]);
  }

  // create a socket
  clientfd = socket(AF_INET,SOCK_STREAM,0);
  if(clientfd<0)
  {
    printf("create socket failed!\n");
    exit(1);
  }
  bzero(&servaddr,sizeof(struct sockaddr_in));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(nPort);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");



  //connet socket 
  int nlength=0;
  dbtime_startTest("Connect & Recv");
  if(connect(clientfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
  {
     printf("server accept failed\n");
     exit(1);
  }

  
  
  int nClose = 0;
  int flag = 0;
  int nCount = 0; 
  int nWrite = 0;
  int nNum = 0;
  long int filesize = 1;
  FILE* fp = NULL;
  while(1)
  {
    char buffer[1024]={0};
    if(nNum >= filesize)
    {
       dbtime_endAndShow();
       dbtime_startTest ("Sleep 5s");
        sleep(5);
	dbtime_endAndShow ();
	dbtime_finalize ();
       fclose(fp);
       printf(" ***********over**********\n");
    }
    
    
    if(0 == flag)
    {
       nClose = read(clientfd,buffer,1023);
       buffer[nClose] = '\0';
       filesize = atol(buffer);
       printf("filesize : %ld\n",filesize);
       fp = fopen("test.txt","w+");
       if(fp == NULL)
       {
	  printf("fail to create file\n");
          break;
       } 
       flag = 1;
       continue;
    }
    nClose = read(clientfd,buffer,1023);
    buffer[nClose] = '\0';
    printf("count:%d\n",nCount++);
    printf("%d \n",nClose);
    if(nClose<0)
    {
       printf("failed to recieve data from server\n");
       break;
    }
    
    nWrite = fwrite(buffer,1,nClose,fp);
    
    if(nWrite==0)
    {
       printf("fail to read file\n");
    }
    fclose(fp);
    fp = fopen("test.txt","a+");
    nNum += nClose;
    printf("nNum :%ld\n",nNum);
  }
  
  close(clientfd);
  return 0;
}
