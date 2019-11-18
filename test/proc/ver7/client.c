#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#include "community.h"

#define MYPORT 4950    /* the port users will be connecting to */

//int sockState; // normal:0, other 1

void sig_pipe(int signo);

void sig_pipe(int signo)
{
    signal(signo, SIG_IGN);
    printf("Received Signal : %d\n", signo);
}


int main(int argc, char *argv[])
{
	int sockfd, sockState;
	struct sockaddr_in serverAddr; /* connector's address information */
	struct hostent *he;
	int state;
	char sndbuf[1024], rcvbuf[100], strbuf[100];
	char *pSndbuf;
	int txlen, rxlen, seqNum, recvLen;

	COMMUNITY_T cm;
	COMMUNITY_T *pCm;

	seqNum = 0;
	sockState = 0;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	serverAddr.sin_family = AF_INET;      // host byte order 
	serverAddr.sin_port = htons(MYPORT);  // short, network byte order 
	inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);	
//	serverAddr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(serverAddr.sin_zero), 8);     // zero the rest of the struct 

	state = connect(sockfd, (struct sockaddr *) &serverAddr,  sizeof(serverAddr));
	
	if(state == -1)
	{
		perror("connect");
		exit(0);
	}

	signal (SIGPIPE, sig_pipe);

    FILE  * fRead;
    char   chBuffer[1024];
	char   chNum[5];  // add
	
    fRead = fopen( "z.log", "r");

	memset(chBuffer, 0x00, sizeof(chBuffer));	
	
	while( fgets(chBuffer, 1024, fRead) )					
	{
	//	fgets(chBuffer, 20, fRead);
		printf("send msg [%s]\n", chBuffer);
		
		
		txlen = Send(sockfd, chBuffer, 73);	
				
		if(txlen > 0)
		{
		//	printf("%s\n", chBuffer );
		}
		else
		{
			close(sockfd);
			exit(1);
		}	
		
	}

	close(sockfd);

	return 0;
}

