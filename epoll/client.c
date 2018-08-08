#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>

#define MAXLINE  10
#define SERV_PROT 8000

int main(void)
{
    struct sockaddr_in seraddr;
    char buf[MAXLINE];
    int connfd, i;
    char ch = 'a';
    connfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&seraddr,sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(SERV_PROT);
    struct in_addr s;
    inet_pton(AF_INET, "127.0.0.1", (void *)&s);
    seraddr.sin_addr=s;
    connect(connfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
    while(1){
        for(i=0;i < MAXLINE/2;i++){
            buf[i] = ch;
        }
        buf[i-1] = '\n';
        ch++;
        for(;i<MAXLINE;i++)
            buf[i] = ch;
        ch++;
        write(connfd, buf, sizeof(buf));
        sleep(5);
    }

    close(connfd);
    return 0;
}