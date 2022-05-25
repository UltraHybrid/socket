#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void error(const char *);
int main(int argc, char *argv[])
{
    int sockfd, newsockfd, servlen, n;
    socklen_t clilen;
    struct sockaddr_un  cli_addr, serv_addr;
    unlink("./mySocket.sock");
    if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
        error("creating socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, "./mySocket.sock");
    servlen=strlen(serv_addr.sun_path) +
            sizeof(serv_addr.sun_family);
    if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0)
        error("binding socket");

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(
        sockfd,(struct sockaddr *)&cli_addr,&clilen);
    if (newsockfd < 0)
        error("accepting");
    int sum = 0;
    int buf_size = 10;
    char buf[buf_size];
    while(1)
    {
        bzero(buf,buf_size);
        n = read(newsockfd, buf, buf_size);
        write(1, buf, n);
        sum += atoi(buf);
        bzero(buf,buf_size);
        sprintf(buf, "%d", sum);
        if(send(newsockfd, buf, buf_size, MSG_DONTWAIT | MSG_NOSIGNAL) < 0)
        {
            close(newsockfd);
        }
    }
    close(sockfd);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
