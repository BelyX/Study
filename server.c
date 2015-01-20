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

	signal(SIGCHLD,sig_handle);//ע����Ϣ������
	while(1)
	{		
		confd = acceptClient(lsnfd,&cliaddr);
		if(confd == -1)
		{
		    continue;
		}
		//TCP connected
		childpid = fork();//��������ӽ������򴴽�һ���ӽ������������ݵĽ���
		if(childpid == 0) //childpidΪ�����ʾ��ǰ�����ӽ�����
		{
			int e = 0;	
			close(lsnfd);//�رո�������ļ���socket����Ϊ�ӽ��̻Ḵ�Ƹ����̵�������Դ
			printf("connect a client!\n");
                        sendmd5(confd,FILENAME);
                        e = send_data(confd,FILENAME); 
                        exit(e);
		}
		close(confd);//��ǰ���ڸ�����.
	}

	close(lsnfd);//�ر�����
	exit(0);
}

