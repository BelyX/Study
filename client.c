#include "socket.h"
#include "fileoperate.h"
#include <time.h>
#include "MD5.h"
#define FILENAME "test.txt"

int main(int argc,char **argv)
{
	char *host = "127.0.0.1";
	int sockfd;
        time_t t_start,t_end;

	if(argc > 2)
	{
		printf("Usage:%ss[hostname or IP address]\n",argv[0]);
		exit(1);
	}
	if(argc == 2)
	{
	     host = argv[1];	
	}
	sockfd = tcp_connect(host);
        t_start = time(NULL);
	if(sockfd < 0)
	{
		printf("\tERROR:Make sure that the server have been in runing.\n");	
		exit(1);
	}

        char buffer[MAXBUF]={0};
        read(sockfd,buffer,MAXBUF);
 
        rcv_data(sockfd,FILENAME);//接收数据函数
        t_end = time(NULL);
       
        char md5[MD5_LEN+1];
        strcpy(md5,GetfileMD5(FILENAME));   
       // printf("buffer: %s\n",buffer); 
        if(strcmp(buffer,md5) == 0)
        {
          printf("the total time :%.0fs\n",difftime(t_end,t_start));
        }
        else
        printf("*******the file has been modified*********\n");   
        close(sockfd);
	exit(0);
}

