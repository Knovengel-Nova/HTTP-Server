#ifndef SERVER_H
#define SERVER_H

#include "client.h"
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

int serverInit(Server *server, int port, int backlog);
int serverStart(Server *server);
int serverAcceptClient(Server *server, Client *client);
int serverDestroy(Server *server);

#endif