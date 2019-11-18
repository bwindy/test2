

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>


 
 
#define MAXLINE 	1024
#define MAX_SOCK  	1024
 

int  maxfdp1;      //�ִ� ���Ϲ�ȣ + 1
int  num_chat = 0; // ä�� ������ ��
int  clisock_list[MAX_SOCK]; // ä�ÿ� ������ ���Ϲ�ȣ ���
int  listen_sock;     // Server listen Socket
 
 
// New chatting adduser 
 void  addClient( int s, struct  sockaddr_in * newcliaddr);
 int   getmax();             // �ִ� ���Ϲ�ȣ ã��
 void  removeClient(int s);  // Chatting remove 
 int   tcp_listen( int host, int port, int backlog); // ���� ���� �� listen
 
 void  errquit( char * mesg) 
 { 
	perror(mesg); 
	exit(1);
 }
 void *myFunc(void *arg);
 
int main( int argc,  char * argv[])
{
	int   accp_sock;
	int   thread_id, i;
	fd_set	read_fds;  // �б⸦ ������  fd_set  ����ü
    pthread_t thread_t;
	struct   sockaddr_in  cliaddr;
    int    addrlen= sizeof(struct sockaddr_in);

	if( argc != 2 )
	{
		printf("���� : %s  port\n", argv[0]);
		exit(0);
		
	}
	
	// tcp_listen(host, port, backlog) �Լ� ȣ��
	listen_sock = tcp_listen( INADDR_ANY, atoi(argv[1]), 5);

	while(1)
	{
		FD_ZERO( &read_fds );
		FD_SET( listen_sock, &read_fds);
		
		
		for( i=0; i<num_chat; i++)
		{
			FD_SET(clisock_list[i], &read_fds);
		}
		
		maxfdp1 = getmax() + 1;		
		
		if( select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0 )
			errquit("select fail");		
		
		if( FD_ISSET(listen_sock, &read_fds)) 
		{
			accp_sock = accept( listen_sock, (struct sockaddr*)&cliaddr, &addrlen);
			
			if(accp_sock == -1)
			{
				printf("accept error");
				return 1;
			} else if (accp_sock != -1) {
			
				printf("accept success");
			
				int   counter  = 0;
				pid_t pid;

				pid = fork();	
				
				
				if( pid == 0 )
				{
					printf( "fork success .... server accept success\n");
						
					thread_id = pthread_create(&thread_t, NULL, myFunc, (void *)&accp_sock);
						
					if(thread_id != 0)
					{
						printf("Thread Create fail.....");
						return 1;
					}
					pthread_detach(thread_t);				
					
				}else if( pid == -1) {
					printf( "fork fail........\n");
					return -1;	
					
				}
			
			//addClient(accp_sock, &cliaddr);
			//send(accp_sock, START_STRING, strlen(START_STRING), 0);
			//printf("%d��° ����� �߰�.\n", num_chat);
			
			if( select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0 )
			errquit("select fail");
			
		}		
		

			
/***********************************			
			switch(pid)
			{
				case -1:
				{
					printf( "fork fail........\n");
					return -1;
				}
				case 0:
				{
					printf( "fork success .... server accept success\n");
					
					thread_id = pthread_create(&thread_t, NULL, myFunc, (void *)&sockfd);
					
					if(thread_id != 0)
					{
						printf("Thread Create fail.....");
						return 1;
					}
					pthread_detach(thread_t);
				}
				default:
				{
					printf( "fork default fail...[%d]...", pid);
					
					while( 1 )
					{
						printf( "�θ�: %d\n", counter++);
						sleep( 1);
					}
				}
			}
**********************************/			

		}
		
	}
}

void *myFunc(void *arg)
{
	fd_set	read_fds;  // �б⸦ ������  fd_set  ����ü
	int sockfd, nbyte; 
	int readn, writen;
	char buf[MAXLINE];
	sockfd = *((int *)arg);

	printf("11111111111...\n");

	FD_ZERO( &read_fds );
	FD_SET( sockfd, &read_fds);
			printf("22222222...\n");
	while(1)
	{
		printf("33333333333...\n");
		if( FD_ISSET(sockfd, &read_fds))
        {
			printf("44444444...\n");		
			nbyte = recv(clisock_list[i], buf, MAXLINE, 0);
			printf("5555555555...\n");	
			
			if( nbyte <= 0 )
			{
				perror("Read Error");
				return NULL;
			}
				
			buf[nbyte] = 0;	
			
			printf("66666666...\n");	
			printf( "read data ...[%s]...", buf);		
		}
/*****************		
		writen = write(sockfd, buf, readn);
		if(readn != writen)
		{
			printf("write error %d : %d\n", readn, writen);
			return NULL;
		}
****************/		
	}
}

// New Chatting add 
void  addClient( int s, struct  sockaddr_in  * newcliaddr )
{
	char buf[20];
	inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
	printf("new client : %s\n", buf);
	
	// Chatting Client add
	clisock_list[num_chat] = s;
	num_chat++;
	
}
 
// Remove Chatting
void  removeClient( int s)
{
	close(clisock_list[s]);
	
	if( s != num_chat-1 )
		clisock_list[s] = clisock_list[num_chat-1];
	
	num_chat--;
	
	printf("ä�� ������ 1�� Ż��. ���� ������ �� = %d\n", num_chat);
}

// �ִ� ���Ϲ�ȣ ã��
int  getmax()
{
	// Minnum ���Ϲ�ȣ�� ���� ���� ������ listen_sock
	int	max = listen_sock;
	
	int i;
	
	for( i= 0; i < num_chat; i++)
	{
		if( clisock_list[i] > max )
		{
			max = clisock_list[i];
		}
	}
	
	return max;
}

int  tcp_listen( int host, int port, int backlog)
{
	int sd;
	struct   sockaddr_in  servaddr;
	
	sd = socket( AF_INET, SOCK_STREAM, 0);
	
	if( sd == -1 )
	{
		perror("socket fail");
		exit(1);
	}
	
	// servaddr  ����ü�� ���� ä��
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(host);
	servaddr.sin_port = htons(port);
	
	if( bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind fail");
		exit(1);
	}
	
	// Client �����û ��ٸ�
	listen( sd, backlog);
	
	return sd;
}


 
/********************************************************


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

 
#define BUFSIZE 30

 

void error_handling(char *message);     // �����ڵ鸵
void z_handler(int sig);                // �ñ��ڵ鷯

 

int main(int argc, char **argv)
{
	int serv_sock;                  // ���� ��ũ����
	int clnt_sock;                  // Ŭ���̾�Ʈ ��ũ����
	struct sockaddr_in serv_addr;   // ���� �ּ�������
	struct sockaddr_in clnt_addr;   // Ŭ���̾�Ʈ �ּ�������

	 
	struct sigaction act;
	int addr_size, str_len, state;
	pid_t pid;
	char message[BUFSIZE];

	
	if(argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = z_handler;
	sigemptyset(&act.sa_mask);      // �ñ׳� �ʱ�ȭ
	act.sa_flags = 0;               // �÷��� �ʱ�ȭ

	
	// �ñ׳� �ڵ鷯 ��� 
	state = sigaction(SIGCHLD, &act, 0);

	if(state != 0)
	{
		puts("sigaction() error\n");
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	 

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}

	if(listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error");
	}

	 
	while(1)
	{

		addr_size = sizeof(clnt_addr);

		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);

		if(clnt_sock == -1)
			continue;

	
		// Ŭ���̾�Ʈ���� ������ ���������� ���� 
		if((pid = fork()) == -1)        // fork ���� ��
		{
			close(clnt_sock);
			continue;
		}
		else if(pid > 0)                // �θ� ���μ����� ���
		{
			puts("���� ����");
			close(clnt_sock);

			continue;
		}
		else                            // �ڽ� ���μ����� ���
		{
			close(serv_sock);

			// �ڽ� ���μ����� ó�� ���� : ������ ���� �� ���� 
			while((str_len = read(clnt_sock, message, BUFSIZE)) != 0)
			{
				write(clnt_sock, message, str_len);
				write(1, message, str_len);
			}

			puts("���� ����");

			close(clnt_sock);

			exit(0);

		}
	}

	return 0;
}

 

void z_handler(int sig)
{
	pid_t pid;
	int ret;
	 
	pid = waitpid(-1, &ret, WNOHANG);
	printf("�Ҹ�� ������ ���μ��� ID: %d\n", pid);
	printf("���� �� ������: %d\n\n", WEXITSTATUS(ret));
}

 
void error_handling(char *message)
{
	fputs(message, stderr);

	fputc('\n', stderr);

	exit(1);
}

 
 ********************************************************/
