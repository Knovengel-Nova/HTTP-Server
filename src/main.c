#include "server.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static Server *globalServer = NULL;

static void handleSignal(int signal){
    (void)signal;

    if(globalServer != NULL){
        globalServer->running = 0;
    }
}

int main(){
    Server server;

    if(initServer(&server, DEFAULT_PORT, DEFAULT_BACKLOG) != 0){
        fprintf(stderr, "Failed to initialize Server@%d\n", DEFAULT_PORT);
        return 1;
    }

    globalServer = &server;

    signal(SIGINT, handleSignal);

    if(startServer(&server) != 0){
        fprintf(stderr, "Failed to Start Server@%d\n", DEFAULT_PORT);
        return 1;
    }

    while(server.running){
        Client client;

        if(acceptClient(&server, &client) != 0){
            continue;
        }

        printf("Client Connected Successfully fd:%d@%d\n", client.socket_fd, client.address.sin_port);

        close(client.socket_fd);
    }

    destroyServer(&server);

    return 0;
}
