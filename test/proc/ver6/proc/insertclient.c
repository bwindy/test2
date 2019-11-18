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
    //printf("Received Signal : %d\n", signo);
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

	FILE  * fRead = fopen("insert_data.txt", "r");
	
//	if ((he = gethostbyname("localhost")) == NULL) {  /* get the host info */
//		perror("gethostbyname");
//		exit(1);
//	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	serverAddr.sin_family = AF_INET;      /* host byte order */
	serverAddr.sin_port = htons(MYPORT);  /* short, network byte order */
	inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);	
//	serverAddr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(serverAddr.sin_zero), 8);     /* zero the rest of the struct */

	

	/*
	flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	*/
	//fcntl(sockfd, F_SETFL, O_NONBLOCK);

	state = connect(sockfd, (struct sockaddr *) &serverAddr,  sizeof(serverAddr));
	
	
	if(state == -1)
	{
		perror("connect");
		exit(0);
	}

	signal (SIGPIPE, sig_pipe);

	char  chBuffer[1024];
	
	while(!feof(fRead)) {
		
		memset( chBuffer, 0x00, sizeof(chBuffer));
		
		fgets(chBuffer, sizeof(chBuffer), fRead);
		

		txlen = Send(sockfd, chBuffer, 10);	
				
		if(txlen > 0)
		{
			printf("%s\n", chBuffer );
		}
		else
		{
			close(sockfd);
			exit(1);
		}		
        
        printf("=== [%s] ===\n", chBuffer);		
	}
	
	close(sockfd);

	return 0;
}

