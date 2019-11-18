#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <unistd.h>

#include <errno.h>
#include "community.h"

void endian(COMMUNITY_T *cm)
{
	cm->seq_num   = htonl(cm->seq_num);
	cm->msg_len   = htonl(cm->msg_len);
	cm->msg_type  = htonl(cm->msg_type);
}

int Recv(int sockfd, int timeout, int readLen, char *buf)
{
	COMMUNITY_T ch;
	int     rv;
    char    *sp;
	sp = buf;

    /* read cmd and length */
    rv = readbytes(sockfd, sp, readLen, timeout);//sizeof(COMMUNITY_T), timeout);

    if (rv <= 0)
    {
        return (rv);
    }
	/*
    else if (rv != )
    {
        return (-1);
    }
	*/
	//printf("Recv:%d\n", rv);

    return rv;
}

int readbytes(int sockfd, char *ps_buf, int rlen, int timeout)
{
    struct  timeval     tout, *ptout;
    fd_set  ibits;
    int     rv, rcnt, nread;
    int     cleft;

	if (timeout > 0)			/* timeout */
	{
		/*
    	tout.tv_sec  = timeout;
        tout.tv_usec = 0;
        */
        tout.tv_sec  = 0;
        tout.tv_usec = timeout;
        ptout = &tout;
    }
    else if (timeout == 0)		/* polling */
    {
        tout.tv_sec  = 0;
        tout.tv_usec = 0;
        ptout = &tout;
    }
	else						/* blocking */
	{
		ptout = NULL;
	}

    rcnt = 0;
    cleft = rlen;

    while( cleft > 0)
    {
    	FD_ZERO(&ibits);
    	FD_SET(sockfd, &ibits);
    	errno = 0;

    	if ( ( rv = select (sockfd+1, &ibits, 0, 0, ptout ) ) < 0 )
    	{
    		//printf("errno:%d\n");
        	return (errno);
    	}
    	else if ( rv == 0 )
    	{
        	return (-1);
    	}

        if ( ( nread = read (sockfd, ps_buf, cleft ) ) > 0 )
        {
            cleft -= nread;
            ps_buf += nread;
			rcnt += nread;
		}
		else if (nread == 0)
		{
			return (-1);
		}
		else
		{
			return (errno);
		}
	}

	if (rcnt > 0)
		return (rcnt);
	else
		return (-1);
}

int Send(int sockfd, char *txdata, int len)
{
	int     nCnt;
	int     nLeft, nWritten;
	char* 	packet = txdata;

	for (nLeft = len; nLeft > 0; )
	{
		nWritten = write(sockfd, packet, nLeft);
		if (nWritten < 0)
		{
			if (errno == EAGAIN || errno == EINTR)
			{
				continue;
			}
			else
			{
				return(-1);
			}
		}
		else if (nWritten == 0)
			break;

		nLeft -= nWritten;
		packet += nWritten;
	}

	return(len - nLeft);
}
