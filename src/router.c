#include "router.h"

#include <string.h>

int routerHandle(HttpRequest *request, HttpResponse *response){
    if(request ==NULL || response ==NULL){
        return -1;
    }

    response->contentType = "text/plain";

    if(strcmp(request->method, "GET") == 0){

        if(strcmp(request->target, "/") == 0){
            response->statusCode = 200;
            response->statusText = "OK";
            response->body = "Hello, World!";
            response->bodyLength = strlen(response->body);

            return 0;
        }

        if(strcmp(request->target, "/hello") == 0){
            response->statusCode = 200;
            response->statusText = "OK";
            response->body = "Hello from /hello!";
            response->bodyLength = strlen(response->body);

            return 0;
        }

        if(strcmp(request->target, "/about") == 0){
            response->statusCode = 200;
            response->statusText = "OK";
            response->body = "About this HTTP server.";
            response->bodyLength = strlen(response->body);

            return 0;
        }
    }

    response->statusCode = 404;
    response->statusText = "Not Found";
    response->body = "404 Not Found";
    response->bodyLength = strlen(response->body);

    return 0;
}