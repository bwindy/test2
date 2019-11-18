 
/**********************************************************
 *
 *  tcp_Client.c
 *  Compile : gcc -o client tcp_Client.c
 *  Using   : ./client  Ip-Address(223.210.20.21) portnumber(50001)
 *
 **********************************************************/
 
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <strings.h>
 #include <fcntl.h>
 #include <sys/socket.h>
 

 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 #include <sys/time.h>
 
 
 #define  MAXLINE   1000
 #define  NAME_LEN  20


/************************************ 
int main( int argc, char * argv[])
{
	char chBuff[1024] = "1234567890";

	FILE *fWrite;

	fWrite = fopen("t.txt", "w");

	int i;

	for( i = 0; i < 10; i++)
		fprintf(fWrite, "%s\n", chBuff);

	fclose(fWrite);

        FILE *fRead;

        char chBuff2[1024];

        fRead = fopen("t.txt", "r");
        memset( chBuff2, 0x00, sizeof(chBuff2));
 
	while(!feof(fRead))
	{
       		
                memset( chBuff2, 0x00, sizeof(chBuff2));
                fgets(chBuff2, 1024, fRead);
                printf("%s\n", chBuff2);
	}
        
        fclose(fRead);

}
*******************************************/

 
 char  *EXIT_STRING = "exit";
 
 // 소켓 생성 및 서버 연결, 생성된 소켓리턴
 int  tcp_connect(int af, char *servip, unsigned short port);
 void  errquit( char * mesg)
 {
	 perror(mesg);
	 exit(1);
	 
 }
 
 int main( int argc, char *argv[])
 {
	 char  bufall[MAXLINE+NAME_LEN],  *bufmsg; 
	 int   maxfdp1, s, namelen;
	 fd_set   read_fds;
	 
	 if( argc != 3)
	 {
		 printf("사용법 : %s server ip port \n", argv[0]);
		 exit(0);
		 
	 }
	 
	 sprintf(bufall, "[%s] :", argv[3]); // bufall 
	 namelen = strlen(bufall);
	 bufmsg = bufall+namelen; 
	 
	 s = tcp_connect( AF_INET, argv[1], atoi(argv[2]));
	 
	 if( s == -1 )
	 {
		 errquit("tcp_connect fail");
	 }
	 
	puts("서버에 접속되었습니다.");
	maxfdp1 = s + 1;
     	 
	FD_ZERO(&read_fds);
	
	
    FILE  * fRead;
    char   chBuffer[1024];

    fRead = fopen( "t.txt", "r");

	char  strBufr[1024] = "aaaaaaaaaaaaaaaa";
	
	while(1)
	{
		FD_SET(0, &read_fds);
		FD_SET(s, &read_fds);
	

		if( select( maxfdp1, &read_fds, NULL, NULL, NULL) < 0 )
		{
		 errquit("select fail");
		}
/***************************		 
		 if( FD_ISSET(s, &read_fds)) 
		 {
			 int nbyte;
			 
			 if( (nbyte = recv(s, bufmsg, MAXLINE, 0)) > 0)
			 {
				 bufmsg[nbyte] = 0;
				 printf("%s \n", bufmsg);
			 }
			 
		 }
***********************/
			 
		memset(bufall, 0x00, sizeof(bufall));

		if( FD_ISSET(s, &read_fds))
		{		
			memcpy( bufall, strBufr, 1024 );
			
			if( send( s, bufall, namelen+strlen(bufmsg), 0) < 0 )
				puts("Error : Write error on socket.");
	    /*
			while( fgets(chBuffer, 20, fRead) )					
			{
	
				fgets(chBuffer, 20, fRead);
				printf("send msg [%s]\n", chBuffer);
				
				if( strlen(chBuffer) != 0 )
					send( s, chBuffer, sizeof(chBuffer),0);
				
				if( strstr( chBuffer, EXIT_STRING) != NULL )
				{
					puts("Good bye.");
					close(s);
					exit(0);
					
				}				
			}
         */
		}
		
/**********************	

		if( FD_ISSET(0, &read_fds))
		{
			if( fgets(bufmsg, MAXLINE, stdin)) 
			{
				if( send( s, bufall, namelen+strlen(bufmsg), 0) < 0 )
					puts("Error : Write error on socket.");
				
				if( strstr( bufmsg, EXIT_STRING) != NULL )
				{
					puts("Good bye.");
					close(s);
					exit(0);
					
				}
				
			}
			
		}
****************/ 
		 
	}
	 
 }
 
 
 int  tcp_connect( int af, char *servip, unsigned short port)
 {
	 struct  sockaddr_in  servaddr;
	 int s;
	 
	 // 소켓생성
	 if((s =socket(af, SOCK_STREAM, 0)) < 0)
	 {
		 
		 return -1;
	 }
	 
	 // 채팅 서버의 소켓 주소 구조체 servaddr 초기화
	 bzero((char *)&servaddr, sizeof(servaddr));
	 servaddr.sin_family = af;
	 inet_pton(AF_INET, servip, &servaddr.sin_addr);
	 servaddr.sin_port = htons(port);
	 
	 // 연결요청
	 if( connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
	 {
		 return -1;
		 
	 }
		 
	 return s;
 }
 
 
