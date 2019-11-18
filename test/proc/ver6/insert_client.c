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

//	while(1)
//  {
    char   chBuffer[1024];
	char   chNum[5];  // add
	
	int i; 
	
	for( i = 1; i < 100; i++)
	{
		memset(chBuffer, 0x00, sizeof(chBuffer));
		memset(chNum,    0x00, sizeof(chNum));	

		char  chPhone[12] = "0103333";
		char  chName[21]  = "HongGilDong";
		char  chCardNum[21] = "11223344556677889900";
		char  chCardInc[21] = "Samsung Card";
	
		sprintf(chNum,  "%04d", i);
		strncat(chPhone, chNum, 4);

		
		strncat(chBuffer, "73",       2);				
		strncat(chBuffer, "71",       2);
		strncat(chBuffer, chPhone,   11);
		strncat(chBuffer, chName,    20);
		strncat(chBuffer, chCardNum, 20);
		strncat(chBuffer, chCardInc, 20);

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
	}
			
			
 	/*	//sleep(3);
		bzero(&rcvbuf, sizeof(rcvbuf));
		
		

		rxlen = Recv(sockfd,  100, sizeof(COMMUNITY_T), rcvbuf);
		if(rxlen <= 0)
		{
			if(sockState == 1)
			{
				printf("socket disconnected\n");
				exit(0);
			}

			if(seqNum > 100) seqNum = 1;
			else seqNum += 1;

			bzero(&strbuf, sizeof(sndbuf));
			
		//	printf("message : ");
		//	fgets(strbuf, sizeof(strbuf), stdin);
			
			sprintf(strbuf, "(%d) msg", seqNum);

			txlen = strlen(strbuf) + sizeof(COMMUNITY_T);
			if (strncmp(sndbuf, "quit", 4) == 0)
			{
				close(sockfd);
				exit(0);
			}

			memset(&cm, 0x00, sizeof(cm));

			cm.seq_num = seqNum;
			cm.msg_len = strlen(strbuf);
			cm.msg_type = 1;

			endian(&cm);

			bzero(&sndbuf, sizeof(sndbuf));
			memcpy(&sndbuf, (char *) &cm, sizeof(cm));
			pSndbuf = (char *)sndbuf+sizeof(cm);
			memcpy(pSndbuf, (char *) &strbuf, cm.msg_len);

			txlen = Send(sockfd, sndbuf, txlen);

			if(txlen > 0)
			{
				printf("<< TX(%02d) : %s\n", cm.msg_len, strbuf);
			}
			else
			{
				if(errno == EPIPE) sockState = 1;
				else printf("errno : %d\n", errno);
			}

		}
		else
		{
			if(sockState == 1)
			{
				printf("socket disconnected\n");
				exit(0);
			}

			pCm = (COMMUNITY_T *)rcvbuf;

			endian(pCm);

			
		//	printf("pCm->seq_num :%d\n", pCm->seq_num);
		//	printf("pCm->msg_len :%d\n", pCm->msg_len);
		//	printf("pCm->msg_type:%d\n", pCm->msg_type);
			

			recvLen = pCm->msg_len;
			bzero(&rcvbuf, sizeof(rcvbuf));
			rxlen = Recv(sockfd, 100, recvLen, rcvbuf);

			if(rxlen > 0)
				printf(">> RX(%02d) : %s\n\n", rxlen, rcvbuf);
		}
		*/
//  }

	close(sockfd);

	return 0;
}

