#include "server.h"
#include "client.h"
#include "connection.h"

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
        Client *client = createClient();

        if(client == NULL){
            fprintf(stderr, "Failed to Create Client\n");
            continue;
        }

        if(acceptClient(&server, client) != 0){
            perror("Accept\n");
            destroyClient(client);
            continue;
        }

        printf("Client Connected Successfully fd: %d\n", client->socket_fd);

        if(connectionStart(client)!=0){
            fprintf(stderr, "Failed to start conncetionHandler\n");
            destroyClient(client);
            continue;
        }



    }

    printf("Shutting down Server @%d\n", DEFAULT_PORT);
    destroyServer(&server);

    return 0;
}
