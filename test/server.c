#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

#define FDCNT 30

typedef struct {
    int fd;
    char ip[20];
} CLIENT;

int clientCount = 0;
int sockfd_connect[FDCNT];
void *client_thread( CLIENT *clientPtr );

int main()
{
    int i;
    int tempfd = 0;
    int clnt_addr_size = 0;
    int sockfd_listen;
    char msg[128];

    struct sockaddr_in server;
    struct sockaddr_in clnt_addr;
    CLIENT client_data[FDCNT] = {0};
    pthread_t  ptid[FDCNT] = {0};

    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(5000);

    if((sockfd_listen = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("fail to call socket()\n");
        return 1;
    }

    if(bind(sockfd_listen, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1) {
        printf("fail to call bind()\n");
    }

    if( listen(sockfd_listen, 5) == -1) {
        printf("fail to call listen()\n");
    }

    clnt_addr_size = sizeof(clnt_addr);

    while(1)
    {
        tempfd = accept(sockfd_listen, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

        if( clientCount == FDCNT )
        {
            printf("sockfd full\n");
            close(tempfd);
            continue;
        }

        if(tempfd < 0)
        {
            printf("fail to call accept() \n");
            continue;
        }

        for( i=0; i<FDCNT; i++)
        {
            if(client_data[i].fd == 0)
            {
                client_data[i].fd = tempfd;
                break;
            }
        }

        strcpy(client_data[i].ip, inet_ntoa(clnt_addr.sin_addr));

        pthread_create( ptid+i, NULL, (void *)client_thread, client_data+i);

        printf("accepted sockfd:%d, clientCount:%d i:%d\n", client_data[i].fd, clientCount, i);
        clientCount++;
    }

    close(sockfd_listen);

    return 0;

}

void * client_thread(CLIENT *clientPtr )
{
    int rst;
    char msg[128];

    while(1)
    {
        rst = read(clientPtr->fd, msg, sizeof(msg));

        if(rst <= 0)
            break;
        else
            printf("[%s] %s\n", clientPtr->ip, msg);

        write(clientPtr->fd, msg, strlen(msg)+1);
    }

    close(clientPtr->fd);
    clientCount--;
    clientPtr->fd = 0;
    printf("close(%s) \n", clientPtr->ip);

    return NULL;
}
