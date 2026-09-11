#include "connection.h"
#include "request.h"
#include "strings.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 8192

static void *connectionHandler(void *arg){
    Client *client = (Client *)arg;

    printf("Handling Client fd: %d\n", client->socket_fd);

    char buffer[BUFFER_SIZE];
    size_t totalBytes = 0;
    ssize_t bytesRead;

    while(totalBytes < BUFFER_SIZE){
        bytesRead = recv(client->socket_fd, buffer + totalBytes, BUFFER_SIZE - totalBytes, 0);

        if(bytesRead < 0){
            perror("recv");
            break;
        }

        if(bytesRead == 0){
            break;
        }

        totalBytes += bytesRead;

        if(findHeaderEnd(buffer, totalBytes) != NULL){
            break;
        }
    }

    if(totalBytes == 0){
        clientDestroy(client);
        return NULL;
    }

    const char *headerEnd = findHeaderEnd(buffer, totalBytes);

    if(headerEnd == NULL){
        printf("HTTP headers too large or incomplete!\n");

        clientDestroy(client);
        return NULL;
    }

    size_t headerLength = (size_t)(headerEnd - buffer) + 4;

    HttpRequest request;

    httpRequestInit(&request);

    if(httpRequestParse(buffer, headerLength, &request) != 0){
        printf("Invalid HTTP request!\n");

        httpRequestDestroy(&request);
        clientDestroy(client);

        return NULL;
    }

    size_t bodyLength = 0;

    const char *contentLength = httpRequestGetHeader(&request, "Content-Length");

    if(contentLength != NULL){
        char *endPointer;

        unsigned long value = strtoul(
            contentLength,
            &endPointer,
            10
        );

        if(*endPointer != '\0'){
            printf("Invalid Content-Length!\n");

            httpRequestDestroy(&request);
            clientDestroy(client);

            return NULL;
        }

        bodyLength = (size_t)value;
    }

    size_t bodyBytesReceived = totalBytes - headerLength;

    while(bodyBytesReceived < bodyLength){

        if(totalBytes >= BUFFER_SIZE){
            printf("Request too large!\n");

            httpRequestDestroy(&request);
            clientDestroy(client);

            return NULL;
        }

        bytesRead = recv(client->socket_fd, buffer + totalBytes, BUFFER_SIZE - totalBytes, 0);

        if(bytesRead < 0){
            perror("recv");

            httpRequestDestroy(&request);
            clientDestroy(client);

            return NULL;
        }

        if(bytesRead == 0){
            printf("Client closed connection before body was complete!\n");

            httpRequestDestroy(&request);
            clientDestroy(client);

            return NULL;
        }

        totalBytes += bytesRead;
        bodyBytesReceived += bytesRead;
    }

    httpRequestDestroy(&request);
    httpRequestInit(&request);

    if(httpRequestParse(buffer, totalBytes, &request) != 0){
        printf("Invalid HTTP request!\n");

        httpRequestDestroy(&request);
        clientDestroy(client);

        return NULL;
    }

    printf("HTTP Request:\n");

    printf("Method: %s\n", request.method);
    printf("Target: %s\n", request.target);
    printf("Version: %s\n\n", request.version);

    printf("Header Count: %zu\n", request.headerCount);

    for(size_t i = 0; i < request.headerCount; i++){
        printf("\t%s: %s\n", request.headers[i].name, request.headers[i].value);
    }

    printf("Body Length: %zu\n", request.bodyLength);

    if(request.body != NULL){
        printf("Body: %.*s\n", (int)request.bodyLength, request.body);
    }

    httpRequestDestroy(&request);
    clientDestroy(client);

    return NULL;
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
