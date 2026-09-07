#include "client.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Client *createClient(){
    Client *client = malloc(sizeof(client));

    if(client == NULL){
        return NULL;
    }

    memset(client, 0, sizeof(client));

    client->socket_fd = -1;

    return client;
}

void destroyClient(Client *client){
    if(client == NULL){
        return;
    }

    if(client->socket_fd != 1){
        close(client->socket_fd);
        client->socket_fd = -1;
    }

    free(client);
}
