#include "errors.h"

#include <string.h>

void httpErrorResponse(HttpResponse *response, int statusCode){
    if(response == NULL){
        return;
    }

    response->contentType = "text/plain";
    
    switch(statusCode){

        case 400:
            response->statusCode = 400;
            response->statusText = "Bad Request";
            response->body = "400 Bad Request";
            break;

        case 404:
            response->statusCode = 404;
            response->statusText = "Not Found";
            response->body = "404 Not Found";
            break;

        case 405:
            response->statusCode = 405;
            response->statusText = "Method Not Allowed";
            response->body = "405 Method Not Allowed";
            break;

        case 500:
            response->statusCode = 500;
            response->statusText = "Internal Server Error";
            response->body = "500 Internal Server Error";
            break;

        default:
            response->statusCode = 500;
            response->statusText = "Internal Server Error";
            response->body = "500 Internal Server Error";
            break;
    }

    response->bodyLength = strlen(response->body);
}