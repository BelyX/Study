#include "socket.h"
#include "fileoperate.h"
#define FILENAME "1.txt"

int main(int argc,char **argv)
{
	int 	lsnfd,confd;
	pid_t 	childpid;
	socklen_t	clilen;
	struct sockaddr_in cliaddr,servaddr;

        lsnfd = Createsockfd(&servaddr);
        Bind(lsnfd,&servaddr);
        Listen(lsnfd,5);

	signal(SIGCHLD,sig_handle);//注册消息处理函数
	while(1)
	{		
		confd = acceptClient(lsnfd,&cliaddr);
		if(confd == -1)
		{
		    continue;
		}
		//TCP connected
		childpid = fork();//如果有连接进来，则创建一个子进程来处理数据的接收
		if(childpid == 0) //childpid为零则表示当前是在子进程里
		{
			int e = 0;	
			close(lsnfd);//关闭父进程里的监听socket，因为子进程会复制父进程的所有资源
			printf("connect a client!\n");
                        sendmd5(confd,FILENAME);
                        e = send_data(confd,FILENAME); 
                        exit(e);
		}
		close(confd);//当前是在父进程.
	}

	close(lsnfd);//关闭连接
	exit(0);
}

