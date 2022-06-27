/*
** EPITECH PROJECT, 2022
** My_messenger
** File description:
** client
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int ac, char **av)
{
    int sockcl = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    if (sockcl == -1) {
        perror("socket()");
        exit(errno);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(av[1]));

    int result_connect = connect(sockcl, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (result_connect < 0) {
        perror("Connection Failed");
        exit(errno);
    }

    char buffer[256];

    ssize_t socket_read = read(sockcl, buffer, 1024);
    printf(buffer);
}