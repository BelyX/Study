#include<socket.h>
int initsocket(sockaddr_in &sockaddr, int nPort)
{
   bzero(&sockaddr,sizeof(struct sockaddr_in));
   sockaddr.sin_family = AF_INET;
   sockaddr.sin_port=htons(nPort);
   sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   return 0;
}


int Connect(sockaddr_in &sockaddr)
{
   int sockfd;
   sockfd = socket(AF_INET,SOCK_STREAM,0);
   if(sockfd<0)
   {
      perror("fail to create socket!\n");
      exit(1);
   }
   return sockfd;
}


int Bind(int sockfd, sockaddr_in sockaddr)
{
   if(bind(sockfd,(struct sockaddr*)&sockaddr,sizeof(struct sockaddr)) == -1)
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
	return 0;
}


int Accept(int listenfd, sockaddr_in sockaddr)
{
   int clientfd = accpet(listenfd, (struct sockadd*)&sockaddr, sizeof(sockaddr));
   if(clientf <0 )
   {
      perror("connfd<0\n");
      exit(1);
   }

   return clientf;
}



int Connect(int serverfd, sockaddr_in serveraddr)
{
   if(connect(serverfd, (struct sockadd*)&serveraddr, sizeof(serveraddr))<0)
   {
      perror("fail to connect server\n");
      exit(1);
   }

   return 0;
}








