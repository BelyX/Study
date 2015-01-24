#include "fileoperate.h"

int sendmd5(int fd,char *filename)
{
    char md5[MD5_LEN+1];
    int nCount = 0;
    strcpy(md5,GetfileMD5(filename));
   // printf("md5: %s\n",md5);
    nCount = write(fd,md5,strlen(md5));
    if(nCount<0)
    {
        printf("fail to send md5\n");
        return -1;
    }
    return 0;
}



ssize_t readn(int fd,void *buf,size_t nbytes)
{
	size_t nleft = nbytes;
	char *ptr = buf;
	ssize_t nread;

	while(nleft > 0)
	{
		if((nread = read(fd,ptr,nleft)) < 0)	
		{
			if(errno == EINTR) nread = 0; //invoke read() again.
			else return (-1);
		}else if(nread == 0)
		{
			break;	//end of file
		}

		nleft -= nread;
		ptr += nread;
	}
	return (nbytes - nleft); 
}

ssize_t writen(int fd,const void *buf,size_t nbytes)
{
	size_t nleft = nbytes;
	const char *ptr = buf;
	ssize_t nwritten;
	
	while( nleft > 0)
	{
		if((nwritten = write(fd,ptr,nleft)) <= 0)	
		{
			if(nwritten < 0 && errno == EINTR)	
			{
				nwritten = 0;	//invoke write() again.
			}else
			{
				return (-1);	//error!
			}
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return (nbytes);
}


/*接收数据的函数参数sockfd是已经建立的socket连接的描述符*/
int rcv_data(int sockfd,char* filename)
{
	size_t e = 0;
	FILE *fd = NULL;
	unsigned int count = 0;
	char buf[MAXLINE + 1] = {0};
	
	if(sockfd < 0)//如果描述小于零则直接退出
	{
		printf("ERROR:bad socket!\n");
		return (-1);
	}
		
	fd = fopen(filename,"w+"); //创建接收文件
	if(fd == NULL) //若创建文件失败则退出
	{
		printf("ERROR:create file failed!\n");
		return (-1);
	}

	while(1)
	{
		count = readn(sockfd,buf,MAXLINE);//从socket连接读取数据
		if(count > 0) //如果count大于零则表示读到了数据
		{
		      
		       e = fwrite(buf,sizeof(char),count,fd);//将数据写到文件
		       if(e != count)
		       {
		      		printf("ERROR:write data to file failed!\n"); 
		      		fclose(fd);
				return (-1);
		       }
		}
		else if(count == 0)//如果count等于零表示数据接收完了
		{
			printf("OK:file transmittion completted!\n");
			fclose(fd);
			return (0);
		}
		else if(count == -1)//如果count等于负一则表示读数据出错了
		{
			printf("ERROR:read data failed!\n");
			fclose(fd);
			return (-2);
		}
	}
}


/*传送数据，这里是用从一个文件读取数据传送给服务器来模拟流媒体数据。*/
int send_data(int sockfd,char* filename)
{
	FILE *fd = NULL;
	unsigned int count = 0;
	char buf[MAXLINE + 1] = {0};

	fd = fopen(filename,"r");
	if(fd == NULL)
	{
		printf("ERROR:Open file 'a' failed.\n");	
		close(sockfd);
		exit(1);
	}
	while((count = fread(buf,sizeof(char),MAXLINE,fd)))//从文件中读取数据
	{
		writen(sockfd,buf,count);
	}
	fclose(fd);

	return (0);
}



















