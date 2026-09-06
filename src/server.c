#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int initServer(Server *server, int port, int backlog){
    if(server == NULL){
        return -1;
    }

    server->port = port;
    server->backlogs = backlog;
    server->running = 0;

    server->socket_fd = -1;

    memset(&server->address, 0, sizeof(server->address));

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(port);

    return 0;
}

int startServer(Server *server){
    if(server == NULL){
        return -1;
    }

    //  creating an actual socket for oue server
    if((server->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Socket");
        return -1;
    }

    int opt = 1;

    //  setSockOpt()
    if(setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
        perror("SetSockOpt");
        close(server->socket_fd);
        server->socket_fd = -1;
        return -1;
    }

    //  bind()
    if(bind(server->socket_fd, (struct sockaddr *)&server->address, sizeof(server->address)) == -1){
        perror("Bind");
        close(server->socket_fd);
        server->socket_fd = -1;
        return -1;
    }

    //listen()
    if(listen(server->socket_fd, server->backlogs) == -1){
        perror("Listen");
        close(server->socket_fd);
        server->socket_fd = -1;
        return -1;
    }
    server->running = 1;
    printf("Server Listening on port %d\n", server->port);

    return 0;
}

int acceptClient(Server *server, Client *client){
    if(server == NULL || client == NULL){
        return -1;
    }

    socklen_t client_len = sizeof(client->address);

    client->socket_fd = accept(server->socket_fd, (struct sockaddr*)&client->address, &client_len);

    if(client->socket_fd == -1){
        perror("Client acceot");
        return -1;
    }

    return 0;
}

int destroyServer(Server *server){
    if(server == NULL){
        return -1;
    }

    if(server->socket_fd != -1){
        close(server->socket_fd);
        server->socket_fd = -1;
        printf("Server Closed Successfully\n");
    }

    return 0;
}
