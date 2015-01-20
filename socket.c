#include "socket.h"


/*消息处理函数*/
void sig_handle(int signo)
{
	pid_t pid;
	int	stat;

	while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
		printf("child %d terminated\n",pid);
	return ;	
}


int initsocket(struct sockaddr_in *sockaddr)
{
   bzero(sockaddr,sizeof(struct sockaddr_in));
   (*sockaddr).sin_family = AF_INET;
   (*sockaddr).sin_port=htons(8888);
   (*sockaddr).sin_addr.s_addr = inet_addr("127.0.0.1");
   return 0;
}


int Createsockfd(struct sockaddr_in *sockaddr)
{
   int sockfd;
   initsocket(sockaddr);
   sockfd = socket(AF_INET,SOCK_STREAM,0);
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
   if(clientfd < 0)
   {
	if(errno == EINTR)
	{
		return -1;
	}
	else	
	{
		printf("ERROR:accept failed!\n");	
		exit(1);
	}
   }
   char *str = inet_ntoa(sockaddr->sin_addr);
   printf("connect from: %s\n", str);

   return clientfd;
}


int tcp_connect(const char *host)
{
	struct hostent *hstent;
	struct servent *svrent;
	struct sockaddr_in sin;
	int s;
	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	
	if((sin.sin_port = htons(8888)) == 0 )
	{
		return -4;	
	}
	hstent = gethostbyname(host);
	if(svrent)
	{
		memcpy(&sin.sin_addr,hstent->h_addr,hstent->h_length);
	}
	else if((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
	{
		return -3;
	}
	s = socket(AF_INET,SOCK_STREAM,0); 
	if(s < 0)
	{
		return -2;	
	}
	if(connect(s,(struct sockaddr *)&sin,sizeof(sin)) < 0)
	{
		return -1;
	}
	return s;
}







