#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
void error(const char *);

int main(int argc, char *argv[])
{
    int opt;
    long timeout;
    while ((opt = getopt(argc, argv, "t:")) != -1)
    {
        if (opt == 't')
            timeout = atoi(optarg) * 1000;
        else
        {
            fprintf(stderr, "Use: %s [-b size] milliseconds timeout\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    int sockfd, servlen,n;
    struct sockaddr_un  serv_addr;

    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, "./mySocket.sock");
    servlen = strlen(serv_addr.sun_path) +
              sizeof(serv_addr.sun_family);
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
        error("Creating socket");
    if (connect(sockfd, (struct sockaddr *)
        &serv_addr, servlen) < 0)
        error("Connecting");
    int buf_size = 10;
    char buffer[buf_size];

    while(fgets(buffer, buf_size , stdin) != NULL)
    {
        write(sockfd, buffer, strlen(buffer));
        n = read(sockfd, buffer, buf_size);
//        write(1, "Server response: ", 17);
//        write(1, buffer, n);
//        write(1, "\n", 2);
        usleep(timeout);
    }
    close(sockfd);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}