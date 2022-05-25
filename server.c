#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const int max_connection_count = 100;

int main(int argc, char *argv[])
{
    int sockets[max_connection_count];
    memset(sockets, -1, max_connection_count * sizeof(int));

    int listenfd, servlen, n;
    struct sockaddr_un serv_addr;
    unlink("./mySocket.sock");
    if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        perror("creating socket");
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, "./mySocket.sock");
    servlen = strlen(serv_addr.sun_path) +
              sizeof(serv_addr.sun_family);
    if (bind(listenfd, (struct sockaddr*) &serv_addr, servlen) < 0)
        perror("binding socket");
    int sum = 0;
    int buf_size = 10;

    listen(listenfd, max_connection_count);

    fd_set ready, ready1;
    FD_ZERO(&ready);
    FD_SET(listenfd, &ready);
    struct timeval tv;
    tv.tv_sec = 100;
    tv.tv_usec = 0;
    int max_fd = listenfd;
    int new_fd;

    int is_first = 1;
    time_t start;

    while (1)
    {
        memcpy(&ready1, &ready, sizeof(ready));
        int redy_count = select(max_fd + 1, &ready1, 0, 0, &tv);
        if (redy_count < 0)
        {
            perror("select");
            continue;
        }
        for (int i = 0; i <= max_fd && redy_count > 0; ++i)
        {
            if (FD_ISSET(i, &ready1))
            {
                redy_count--;
                if (i == listenfd)
                {
                    do
                    {
                        if(is_first == 1)
                        {
                            is_first = 0;
                            start = time(NULL);
                        }
                        new_fd = accept(listenfd, NULL, NULL);
// print desc number and heap border
//                        char buf[20];
//                        bzero(buf, new_fd);
//                        sprintf(buf, "%d", new_fd);
//                        write(1, "desc num: ", 10);
//                        write(1, buf, 4);
//                        write(1, ".\n", 2);
//                        bzero(buf, new_fd);
//                        write(1, "heap: ", 6);
//                        int a = (int) sbrk(0);
//                        sprintf(buf, "%d", a);
//                        write(1, buf, 10);
//                        write(1, ".\n", 2);

                        if (new_fd > max_fd)
                            max_fd = new_fd;
                        FD_SET(new_fd, &ready);
                    } while (new_fd == -1);
                } else
                {
                    char buf[buf_size];
                    bzero(buf, buf_size);
                    n = read(i, buf, buf_size);
                    //print the number read
//                    write(1, buf, n);
                    sum += atoi(buf);
                    bzero(buf, buf_size);
//                    sprintf(buf, "%d", sum);
                    if (send(i, buf, buf_size, MSG_DONTWAIT | MSG_NOSIGNAL) < 0)
                    {
                        close(i);
                        FD_CLR(i, &ready);
                        time_t f = time(NULL);
                        printf("time: %f\n", difftime(f, start) / 1000);
                    }
                }
            }
        }
    }
    return 0;
}
