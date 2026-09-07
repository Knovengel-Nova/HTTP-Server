#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

typedef struct{
    int socket_fd;
    struct sockaddr_in address;
} Client;

Client *createClient();
void destroyClient(Client *client);

#endif