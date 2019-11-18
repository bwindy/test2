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

#define MYPORT 4950    /* the port users will be connecting to */

#define MAXBUFLEN 1024

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
	
	
   FILE  *pLog = fopen ("z.log", "a");		
   
   
   
	
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
			memset( rcvbuf, 0x00, sizeof(rcvbuf));
			rxlen = Recv(cliSockfd, 100, 10, rcvbuf);
		
			if( rxlen <= 0 ) {
	
				if (pLog) {
					printf("$$$$    %d\n", rxlen);
					fprintf (pLog, "FILE rxlen %d\n", rxlen);
					fprintf (pLog, "FILE buffer %s\n", rcvbuf);
				} /* endif */
				
				printf("=== [%s]\n", rcvbuf );

                continue;				
			//	shutdown(cliSockfd, 2);
			//	close(cliSockfd);
             //	break;	
			} else if( rxlen > 0 ) {
				
				if (pLog) {
					printf("$$$$    %d\n", rxlen);
				    printf("=== [%s]\n", rcvbuf );					
					fprintf (pLog, "FILE rxlen %d\n", rxlen);
					fprintf (pLog, "FILE buffer %s\n", rcvbuf);
				} /* endif */			
				
			}
			
		/*	
        	//sleep(3);
            memset(rcvbuf, 0x00, sizeof(rcvbuf));
			rxlen = Recv(cliSockfd, 100, sizeof(COMMUNITY_T), rcvbuf);
            if (rxlen<= 0)
            {
				
			//	shutdown(cliSockfd, 2);
            //  close(cliSockfd);
            // 	break;
				
				//printf("async message send!\n");

				time_t tt;
				time(&tt);

				if(tt % 5)
				{
					memset(&rcvbuf, 0x00, sizeof(rcvbuf));
					sprintf(rcvbuf, "ASNYC(%d)", tt);
					txlen = strlen(rcvbuf);

					memset(&cm, 0x00, sizeof(COMMUNITY_T));
					cm.seq_num = tt;
					cm.msg_len = txlen;
					cm.msg_type= 2;

					endian(&cm);

					memset(sndbuf, 0x00, sizeof(sndbuf));
					memcpy(&sndbuf, (char *)&cm, sizeof(cm));
					pSndbuf = (char *) sndbuf + sizeof(cm);
					memcpy(pSndbuf, (char *)&rcvbuf, txlen);

					Send(cliSockfd, sndbuf, cm.msg_len + sizeof(cm));
					printf(">> TX(%02d) : %s\n\n", cm.msg_len, sndbuf+sizeof(cm));
				}

				continue;
            }
			else
			{
				pCm = (COMMUNITY_T *)rcvbuf;

				endian(pCm);

				rxlen = Recv(cliSockfd, 100, pCm->msg_len, rcvbuf);
            	if (rxlen<= 0)
            	{
					shutdown(cliSockfd, 2);
                	close(cliSockfd);
               		continue;
            	}

				printf("<< RX(%02d) : %s\n", rxlen, rcvbuf);

				if (strncmp(rcvbuf, "quit", 4) == 0)
				{
					write(cliSockfd, "bye bye", 8);
					break;
				}

				memset(&cm, 0x00, sizeof(COMMUNITY_T));
				if(seqNum > 100) seqNum = 1;
				else seqNum += 1;
				cm.seq_num = seqNum;
				cm.msg_len = rxlen;
				cm.msg_type= 1;

				endian(&cm);

				memset(sndbuf, 0x00, sizeof(sndbuf));
				memcpy(&sndbuf, (char *)&cm, sizeof(cm));
				pSndbuf = (char *) sndbuf + sizeof(cm);
				memcpy(pSndbuf, (char *)&rcvbuf, sizeof(cm));

				Send(cliSockfd, sndbuf, cm.msg_len + sizeof(cm));
				printf(">> TX(%02d) : %s\n\n", cm.msg_len, sndbuf+sizeof(cm));
			}
			*/
        }

		shutdown(cliSockfd, 2);
		close(cliSockfd);
    }

	shutdown(cliSockfd, 2);
	close(serSockfd);
	
	
	fclose (pLog);
	
	return 0;
}

