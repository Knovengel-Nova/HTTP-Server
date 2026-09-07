#include "connection.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static void *connectionHandler(void *arg){
    Client *client = (Client *)arg;

    printf("Handling Client fd: %d\n", client->socket_fd);

    char buffer[4096];

    ssize_t bytesRead = recv(client->socket_fd, buffer, sizeof(buffer)-1, 0);

    if(bytesRead > 0){
        buffer[bytesRead] = '\0';

        printf("Received from fd %d:\n%s\n", client->socket_fd, buffer);

    }

    destroyClient(client);
}

int connectionStart(Client *client){
    if(client == NULL){
        return -1;
    }

    pthread_t thread;

    if((pthread_create(&thread, NULL, connectionHandler, client)) != 0){
        return -1;
    }

    pthread_detach(thread);

    return 0;
}
