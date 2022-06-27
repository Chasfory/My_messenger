/*
** EPITECH PROJECT, 2022
** My_messenger
** File description:
** server
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

int main(int ac, char **av)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    int *all_connections;
    int tmp = 0;

    if (sockfd == -1)
    {
        perror("socket()");
        exit(errno);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(av[1]));

    int result = bind(sockfd, (struct sockaddr *)&address, sizeof(address));

    if (result < 0)
    {
        perror("bind failed");
        return 1;
    }

    int result_listen = listen(sockfd, 2);

    if (result_listen < 0)
    {
        perror("listen");
        return 1;
    }

    int addrlen = sizeof(address);
    all_connections = NULL;
    while (1)
    {
        fd_set set;
        struct timeval timeout;
        int rv;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        timeout.tv_sec = 20;
        timeout.tv_usec = 0;
        for (int i = 0; i < tmp; ++i)
        {
            FD_SET(all_connections[i], &set);
        }
        rv = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
        if (rv == -1)
        {
            perror("select");
            return 1;
        }
        if (FD_ISSET(sockfd, &set))
        {
            int new_fd = accept(sockfd, (struct sockaddr *)&address, &addrlen);
            if (new_fd >= 0)
            {
                all_connections = realloc(all_connections, sizeof(int) * (tmp + 1));
                all_connections[tmp] = new_fd;
                tmp++;
                for (int k = 0; k < tmp; k++)
                {
                    dprintf(all_connections[k], "%d s'est connecté\n", new_fd);
                }
            }
        }
        for (int i = 0; i < tmp; ++i)
        {
            if (FD_ISSET(all_connections[i], &set))
            {
                char buffer[256];
                ssize_t rd = read(all_connections[i], buffer, sizeof(buffer));
                if (rd == -1)
                {
                    return 1;
                }
                if (rd == 0)
                {
                    for (int k = 0; k < tmp; k++)
                    {
                        dprintf(all_connections[k], "%d s'est deconnecté\n", all_connections[i]);
                    }
                    memmove(&all_connections[i], &all_connections[i + 1], (tmp - i - 1) * sizeof(int));
                    tmp--;
                }
                else
                {
                    for (int j = 0; j < tmp; j++)
                    {
                        if (i != j)
                        {

                            ssize_t wd = dprintf(all_connections[j], "#%d: %.*s", all_connections[i], rd, buffer);
                            if (wd == -1)
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < tmp; i++)
    {
        close(all_connections[i]);
    }
}