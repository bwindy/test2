#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "community.h"
#include "insert.h"

#define MYPORT 4950    /* the port users will be connecting to */

#define MAXBUFLEN 1024

int  CheckRecv( int sock, char *sBuffer, int nRecvSize, int nFlag);


int main()
{
	int serSockfd, cliSockfd;
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in clientaddr; /* connector's address information */
	struct linger li;
	int addrLen;
	int state, rxlen, txlen, seqNum;
	char rcvbuf[MAXBUFLEN], sndbuf[MAXBUFLEN];
	char *pIP, *pSndbuf;

	char     chPhone[11];
	char     chName[20]; 
	char     chCardNum[20]; 
	char     chCardInc[20];	
	
	COMMUNITY_T cm;
	COMMUNITY_T *pCm;

	addrLen = sizeof(clientaddr);
	seqNum = 0;

	if ((serSockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
	//my_addr.sin_addr.s_addr = inet_addr("211.55.81.90"); /* auto-fill with my IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero the rest of the struct */
	
	/* set linger */
	li.l_onoff = 1;
	li.l_linger = 0;
	if (setsockopt(serSockfd, SOL_SOCKET, SO_LINGER, (char *)&li, sizeof(struct linger)) < 0)
	{
		perror("setsockopt");
		exit(0);
	}
	
	if (bind(serSockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))  == -1) {
		perror("bind");
		exit(0);
	}

	state = listen(serSockfd, 5);
    if (state == -1)
    {
        perror("listen error : ");
        exit(0);
    }
	
	FILE  *pLog = fopen ("xx.log", "a");		
	
    while(1)
    {
		
        cliSockfd = accept(serSockfd, (struct sockaddr *)&clientaddr, &addrLen);
        if (cliSockfd == -1)
        {
            perror("Accept error : ");
            exit(0);
        }

		pIP = inet_ntoa(clientaddr.sin_addr);
	
		while(1)
        {
			memset( rcvbuf,    0x00, sizeof(rcvbuf));
			memset( chPhone,   0x00, sizeof(chPhone));
			memset( chName,    0x00, sizeof(chName));
			memset( chCardNum, 0x00, sizeof(chCardNum));
			memset( chCardInc, 0x00, sizeof(chCardInc));			
			
			rxlen = CheckRecv( cliSockfd, rcvbuf, 2, 0);			
//			rxlen = Recv(cliSockfd, 100, 73, rcvbuf);
		
			if( rxlen <= 0 ) {
				if (pLog) {
					printf("%s\n", rcvbuf);
					fprintf (pLog, "%s\n", rcvbuf);
				} /* endif */
				
				shutdown(cliSockfd, 2);
				close(cliSockfd);
				break;	
			} else if( rxlen > 0 ) {
				if (pLog) {
					printf("%s\n", rcvbuf);
					fprintf (pLog, "%s\n", rcvbuf);
					
					memcpy( chPhone,   rcvbuf,                sizeof(chPhone));
					memcpy( chName,    rcvbuf+HANDPHONESIZE,  sizeof(chName));
					memcpy( chCardNum, rcvbuf+NAMESIZE,       sizeof(chCardNum));
					memcpy( chCardInc, rcvbuf+CARDNUMBERSIZE, sizeof(chCardInc));
					
					insertCustomer(chPhone, chName, chCardNum, chCardInc);
				//	deleteCustomer(chPhone);
					
				} /* endif */			
			} 
        }

		shutdown(cliSockfd, 2);
		close(cliSockfd);
    }

	shutdown(cliSockfd, 2);
	close(serSockfd);
	
	fclose (pLog);
	
	return 0;
}

int  CheckRecv( int sock, char *sBuffer, int nRecvSize, int nFlag)
{
	int nCheck = 0;

	for( ; ; )
	{
		if( nFlag != 2) {
			memset( sBuffer, 0x00, sizeof(sBuffer));	// add
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
			
			memset( sBuffer, 0x00, sizeof(sBuffer));	// add 		
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


