#include "response.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void httpResponseInit(HttpResponse *response){
    if(response == NULL){
        return;
    }

    response->statusCode = 200;
    response->statusText = "OK";

    response->contentType = "text/plain";

    response->body = NULL;
    response->bodyLength = 0;
}

int httpResponseSend(Client *client, HttpResponse *response){
    if(client == NULL || response == NULL){
        return -1;
    }

    char header[1024];

    int headerLength = snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\n" "Content-Length: %zu\r\n" "Content-Type: %s\r\n" "Connection: close\r\n" "\r\n", response->statusCode, response->statusText, response->bodyLength, response->contentType);

    if(headerLength < 0 || (size_t)headerLength >= sizeof(header)){
        return -1;
    }

    if(send(client->socket_fd, header, (size_t)headerLength, 0) < 0){
        return -1;
    }

    if(response->bodyLength > 0 && response->body != NULL){
        if(send(client->socket_fd, response->body, response->bodyLength, 0) < 0){
            return -1;
        }
    }

    return 0;
}