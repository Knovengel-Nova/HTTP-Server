#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define DEFAULT_PORT 4892
#define DEFAULT_BACKLOG 10

typedef struct {
    int socket_fd;
    int port;
    int backlogs;

    int running;

    struct sockaddr_in address;
} Server;

typedef struct{
    int socket_fd;

    struct sockaddr_in address;
} Client;

int initServer(Server *server, int port, int backlog);
int startServer(Server *server);
int acceptClient(Server *server, Client *client);
int destroyServer(Server *server);

#endif