#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct {
	int seq_num;
	int msg_len;
	int msg_type;	/* normal:1, async:2 */
} COMMUNITY_T;

void endian(COMMUNITY_T *cm);
int Recv(int sockfd, int timeout, int readLen, char *buf);
int readbytes(int sockfd, char *ps_buf, int rlen, int timeout);
int Send(int sockfd, char *txdata, int len);
