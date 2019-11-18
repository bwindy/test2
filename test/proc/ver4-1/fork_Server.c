

/********************************************************

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
 

int  maxfdp1;      //최대 소켓번호 + 1
int  num_chat = 0; // 채팅 참가자 수
int  clisock_list[MAX_SOCK]; // 채팅에 참가자 소켓번호 목록
int  listen_sock;     // Server listen Socket
 
 
// New chatting adduser 
 void  addClient( int s, struct  sockaddr_in * newcliaddr);
 int   getmax();             // 최대 소켓번호 찾기
 void  removeClient(int s);  // Chatting remove 
 int   tcp_listen( int host, int port, int backlog); // 소켓 생성 및 listen
 
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
	fd_set	read_fds;  // 읽기를 감지할  fd_set  구조체
    pthread_t thread_t;
	struct   sockaddr_in  cliaddr;
    int    addrlen= sizeof(struct sockaddr_in);

	if( argc != 2 )
	{
		printf("사용법 : %s  port\n", argv[0]);
		exit(0);
		
	}
	
	// tcp_listen(host, port, backlog) 함수 호출
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
						

					thread_id = pthread_create(&thread_t, NULL, &myFunc, (void *)&accp_sock);
						
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
			//printf("%d번째 사용자 추가.\n", num_chat);
			
			if( select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0 )
			errquit("select fail");
			
		}		
		

		

		}
		
	}
	
	close(listen_sock);
	
}




void *myFunc(void *arg)
{
	fd_set	read_fds;  // 읽기를 감지할  fd_set  구조체
	int sockfd, nbyte; 
	int readn, writen;
	char buf[MAXLINE];
	sockfd = (int)*((int *)arg);

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
			nbyte = recv(sockfd, buf, MAXLINE, 0);
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

//		writen = write(sockfd, buf, readn);
//		if(readn != writen)
//		{
//			printf("write error %d : %d\n", readn, writen);
//			return NULL;
//		}

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
	
	printf("채팅 참가자 1명 탈퇴. 현재 참가자 수 = %d\n", num_chat);
}

// 최대 소켓번호 찾기
int  getmax()
{
	// Minnum 소켓번호는 가장 먼저 생성된 listen_sock
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
	
	// servaddr  구조체의 내용 채팅
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(host);
	servaddr.sin_port = htons(port);
	
	if( bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind fail");
		exit(1);
	}
	
	// Client 연결요청 기다림
	listen( sd, backlog);
	
	return sd;
}

********************************************************/
 
/********************************************************/

/**************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

 
#define BUFSIZE 1024

 

void error_handling(char *message);     // 에러핸들링
void z_handler(int sig);                // 시그핸들러

 

int main(int argc, char **argv)
{
	int serv_sock;                  // 서버 디스크립터
	int clnt_sock;                  // 클라이언트 디스크립터
	struct sockaddr_in serv_addr;   // 서버 주소포인터
	struct sockaddr_in clnt_addr;   // 클라이언트 주소포인터

	 
	struct sigaction act;
	int addr_size, str_len, state;
	pid_t pid;
	char message[BUFSIZE];
   /// add  .....
   
   	fd_set	read_fds;  // 읽기를 감지할  fd_set  구조체
	
	
	if(argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = z_handler;
	sigemptyset(&act.sa_mask);      // 시그널 초기화
	act.sa_flags = 0;               // 플래그 초기화

	
	// 시그널 핸들러 등록 
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

	printf("11111111111...\n");
	
	if(listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error");
	}

	printf("2222222222222222...\n");
	
	while(1)
	{

		addr_size = sizeof(clnt_addr);

		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);

			printf("3333333333333333...\n");
			
			
		if(clnt_sock == -1)
			continue;

	
		// 클라이언트와의 연결을 독립적으로 생성 
		if((pid = fork()) == -1)        // fork 실패 시
		{
			close(clnt_sock);
			continue;
		}
		else if(pid > 0)                // 부모 프로세스인 경우
		{
			puts("연결 생성");
			close(clnt_sock);

			continue;
		}
		else if( pid == 0 )                         // 자식 프로세스인 경우
		{
			close(serv_sock);

	
	
			// 자식 프로세스의 처리 영역 : 데이터 수신 및 전송 
			while(1)
			{
		printf("44444444444444444444...\n");			
				memset(message, 0x00, sizeof(message));
				str_len = read(clnt_sock, message, BUFSIZE);
				printf( "read data ...[%s]...", message);
			//	write(clnt_sock, message, str_len);
			//	write(1, message, str_len);
			}

			puts("연결 종료");

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
	printf("소멸된 좀비의 프로세스 ID: %d\n", pid);
	printf("리턴 된 데이터: %d\n\n", WEXITSTATUS(ret));
}

 
void error_handling(char *message)
{
	fputs(message, stderr);

	fputc('\n', stderr);

	exit(1);
}
**************************************************/




#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <strings.h>
 #include <fcntl.h>
 #include <sys/socket.h>
 #include <sys/file.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 
 
 #define  MAXLINE   511
 #define  MAX_SOCK  1024
 
 
 char  * EXIT_STRING = "exit";  // client 종료 요청 문자열
 char  * START_STRING = "Connected to char_server \n"; // client 환영 메시지
 
 int  maxfdp1;      //최대 소켓번호 + 1
 int  num_chat = 0; // 참가자 수
 int  clisock_list[MAX_SOCK]; // 참가자 소켓번호 목록
 int  listen_sock;     // Server listen Socket
 
 // New chatting adduser 
 void  addClient( int s, struct  sockaddr_in * newcliaddr);
 int   getmax();    // 최대 소켓번호 찾기
 void  removeClient(int s);  // Chatting remove 
 int tcp_listen( int host, int port, int backlog); // 소켓 생성 및 listen
 void  errquit( char * mesg) 
 { 
	perror(mesg); 
	exit(1);
 }

  
int  CheckRecv( int nSock, char *sBuffer, int nRecvSize, int nFlag);
 void *myFunc(void *arg);

int main( int argc,  char * argv[])
{
	struct	sockaddr_in   cliaddr;
	char	buf[MAXLINE+1];
	int    i, j, nbyte, accp_sock, addrlen= sizeof(struct sockaddr_in);
	fd_set	read_fds;  // 읽기를 감지할  fd_set  구조체
	pid_t pid;  // add
	
	if( argc != 2 )
	{
		printf("사용법 : %s  port\n", argv[0]);
		exit(0);
	}
	
	// tcp_listen(host, port, backlog) 함수 호출
	listen_sock = tcp_listen( INADDR_ANY, atoi(argv[1]), 5);
	
   FILE  *pLog = fopen ("t.log", "a");	
	
	while(1)
	{
		FD_ZERO( &read_fds );
		FD_SET( listen_sock, &read_fds);
		
		for( i=0; i<num_chat; i++)
		{
			FD_SET(accp_sock, &read_fds);
			printf("FD_SET(accp_sock, &read_fds  0000 %d  \n", num_chat);
		}
		
		maxfdp1 = getmax() + 1;
//		puts("wait for client");
		
		if( select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0 ){
			
			errquit("select fail");
		}
		
		if( FD_ISSET(listen_sock, &read_fds)) 
		{
			accp_sock = accept( listen_sock, (struct sockaddr*)&cliaddr, &addrlen);
			
			if( accp_sock == -1 )
				errquit("accept fail");
			
		//	addClient(accp_sock, &cliaddr);
		//	send(accp_sock, START_STRING, strlen(START_STRING), 0);
			printf("%d User Add.\n", num_chat);
			
		}
		
		// 클라이언트와의 연결을 독립적으로 생성 
		pid = fork();        
		
		if(pid == -1)                // fork 실패 시
		{
			puts("Connect Create.");
			close(accp_sock);

			continue;
		}		
		else if(pid == 1)                // 부모 프로세스인 경우
		{
			puts("Connect Create.");
			close(accp_sock);

			continue;
		}
		else if( pid == 0 )                         // 자식 프로세스인 경우
		{
			close(listen_sock);
			
	printf("===== 11111111111 ==== \n");
			if( FD_ISSET(accp_sock, &read_fds)) 	
			{
				                                   //2
												   
			//   BOOL  bFlag = 0;
				
				nbyte = recv(accp_sock, buf, 14, 0);
				
				printf("===== buf [%s] ==== \n", buf);
			//	nbyte = recv(clisock_list[i], buf, MAXLINE, 0);				
			//	nbyte = CheckRecv( clisock_list[i], buf, 2, 0);
				
				
				if( nbyte <= 0 )
				{
				//	removeClient(i);  // Client end
					
				//	bFlag = 1;
					continue;
				}
				
				buf[nbyte] = 0;
				
	
				// 종료문자 처리
			    if( strstr( buf, EXIT_STRING) != NULL) 
                {
				//	removeClient(i);   // Client end 
				}					
		/**********************88
				// All Client Message Send 
				for( j = 0; j < num_chat; j++)
					send( clisock_list[i], buf, nbyte, 0);
		*************************/
	
	
			   if (pLog) {

				printf("=======    %s\n", buf);
				  fprintf (pLog, "%s\n", buf);

				printf("#######    %s\n", buf);		
			   } /* endif */
	
				printf("%s\n", buf);
			}

		}
		
		
  /*
        // 클라이언트가 보낸 메시지를 모든 클라이언트에게 전송
     //   for( i = 0; i < num_chat ; i++ )
     //   {
	//		if( FD_ISSET(clisock_list[i], &read_fds)) 
			if( FD_ISSET(accp_sock, &read_fds)) 	
			{
				                                   //2
												   
			//   BOOL  bFlag = 0;
				
				nbyte = recv(accp_sock, buf, 14, 0);
			//	nbyte = recv(clisock_list[i], buf, MAXLINE, 0);				
			//	nbyte = CheckRecv( clisock_list[i], buf, 2, 0);
				
				
				if( nbyte <= 0 )
				{
				//	removeClient(i);  // Client end
					
				//	bFlag = 1;
					continue;
				}
				
				buf[nbyte] = 0;
				
	
				// 종료문자 처리
			    if( strstr( buf, EXIT_STRING) != NULL) 
                {
				//	removeClient(i);   // Client end 
				}					
		
				// All Client Message Send 
			//	for( j = 0; j < num_chat; j++)
			//		send( clisock_list[i], buf, nbyte, 0);
	
	
	
			   if (pLog) {

				printf("=======    %s\n", buf);
				  fprintf (pLog, "%s\n", buf);

				printf("#######    %s\n", buf);		
			   } // endif 
	
				printf("%s\n", buf);
			}
		*/	
		//}  // end of for
		
	}  // end of while
	
	
	fclose (pLog);
	
	
	return 0;
	
} 


// New Client add 
void  addClient( int s, struct  sockaddr_in  * newcliaddr )
{
	char buf[20];
	inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
	printf("new client : %s\n", buf);
	
	// Client add
	//clisock_list[num_chat] = s;
	num_chat++;
	
}
 
// Remove Client
void  removeClient( int s)
{
	close(clisock_list[s]);
	
	if( s != num_chat-1 )
		clisock_list[s] = clisock_list[num_chat-1];
	
	num_chat--;
	
	printf("Client 1 User Socket Close. Current User = %d\n", num_chat);
}

// 최대 소켓번호 찾기
int  getmax()
{
	
	// Minnum 소켓번호는 가장 먼저 생성된 listen_sock
	int   max = listen_sock;
	
	int  i;
	
	for( i= 0; i < num_chat; i++)
	{
		if( clisock_list[i] > max )
		{
			max = clisock_list[i];
		}
		
	}
	
	max = num_chat;
	
	return max;

}

// listen 소켓 생성 및  listen
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
	
	// servaddr  구조체의 내용 채팅
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(host);
	servaddr.sin_port = htons(port);
	
	if( bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind fail");
		exit(1);
	}
	
	// Client 연결요청 기다림
	listen( sd, backlog);
	
	return sd;
	
}
 
int  CheckRecv( int sock, char *sBuffer, int nRecvSize, int nFlag)
{
	int nCheck = 0;

	for( ; ; )
	{
		if( nFlag != 2) {
			nCheck = recv(sock, sBuffer, nRecvSize, 0); 
				
			if( nCheck > 0) {
				nFlag = 1;
			}
				
			if( nCheck <= 0 )
			{
				close(sock);  // Client end
				break;
			}
				
			nRecvSize = atoi(sBuffer);
		}
			
		if( nFlag != 2) {
			nCheck = recv(sock, sBuffer, nRecvSize, 0);
				
			if( nCheck > 0) {
				nFlag = 2;
			}
				
			if( nCheck <= 0 )
			{
				close(sock);  // Client end
				break;
			}				
		}

		if( nFlag == 2) {
			break;
		}		
	}
	
	return nCheck;
}

void *myFunc(void *arg)
{
	fd_set	read_fds;  // 읽기를 감지할  fd_set  구조체
	int sockfd, nbyte; 
	int readn, writen;
	char buf[MAXLINE];
	sockfd = (int)*((int *)arg);

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
			nbyte = recv(sockfd, buf, MAXLINE, 0);
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

//		writen = write(sockfd, buf, readn);
//		if(readn != writen)
//		{
//			printf("write error %d : %d\n", readn, writen);
//			return NULL;
//		}

	}
}

 /********************************************************/
