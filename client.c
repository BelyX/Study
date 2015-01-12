#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dbtime.h"
#include "socket.h"
#include "fileoperate.h"

int main(int argc, char **argv)
{
  struct sockaddr_in servaddr;

  int clientfd;

  // create a socket
  clientfd = Createsockfd(AF_INET,SOCK_STREAM);
  initsocket(&servaddr,argc,argv);

  //connet socket
  dbtime_startTest("Connect & Recv");
  connectClient(clientfd,&servaddr);

  int nClose = 0;
  int flag = 0;
  int nCount = 0; 
  int nWrite = 0;
  long int nNum = 0;
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

       if((fp = openfile("test.txt","w+")) == NULL)
       {
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
       printf("fail to write file\n");
    }
    fclose(fp);

    fp = fopen("test.txt","a+");
    nNum += nClose;
    printf("nNum :%ld\n",nNum);
  }
  
  close(clientfd);
  return 0;
}
