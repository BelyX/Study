#include "socket.h"

int initsocket(struct sockaddr_in *sockaddr, int argc, char** argv)
{
   int nPort=SERVER_PORT;
   if(argc>1)
   {
	nPort = atoi(argv[1]);
   }
   bzero(sockaddr,sizeof(struct sockaddr_in));
   (*sockaddr).sin_family = AF_INET;
   (*sockaddr).sin_port=htons(nPort);
   (*sockaddr).sin_addr.s_addr = inet_addr("127.0.0.1");
   return 0;
}


int Createsockfd(int af,int type)
{
   int sockfd;
   sockfd = socket(af,type,0);
   if(sockfd<0)
   {
      perror("fail to create socket!\n");
      exit(1);
   }
   return sockfd;
}


int Bind(int sockfd, struct sockaddr_in* sockaddr)
{
   if(bind(sockfd,(struct sockaddr*)sockaddr,sizeof(struct sockaddr)) == -1)
   {
      perror("server fail to bind\n");
      exit(1);
   }
   return 0;
}


int Listen(int sockfd, int listenNum)
{
	if(listen(sockfd,listenNum) == -1)
	{
		perror("server fail to listen\n");
		exit(1);
	}
        printf("server listen succeed\n");
	return 0;
}


int acceptClient(int listenfd, struct sockaddr_in* sockaddr)
{
   int nlength = sizeof(*sockaddr);
   int clientfd = accept(listenfd, (struct sockaddr*)sockaddr, &nlength);
   if(clientfd <0 )
   {
      perror("connfd<0\n");
      exit(1);
   }
   char *str = inet_ntoa(sockaddr->sin_addr);
   printf("connect from: %s\n", str);

   return clientfd;
}



int connectClient(int serverfd, struct sockaddr_in* serveraddr)
{
   int nlength = sizeof(*serveraddr);
   if(connect(serverfd, (struct sockaddr*)serveraddr, &nlength)<0)
   {
      perror("fail to connect server\n");
      exit(1);
   }

   return 0;
}








