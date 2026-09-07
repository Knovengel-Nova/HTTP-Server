#include "http.h"

#include <stdlib.h>
#include <string.h>

void httpRequestInit(HttpRequest *request){
    if(request == NULL){
        return;
    }

    request->method = NULL;
    request->target = NULL;
    request->version = NULL;

    request->headers = NULL;
    request->headerCount = 0;

    request->body = NULL;
    request->bodyLength = 0;
}

void httpRequestDestroy(HttpRequest *request){

}

int httpParseRequest(const char* data, size_t length, HttpRequest *request){
    if(request == NULL){
        return;
    }

    free(request->method);
    free(request->target);
    free(request->version);

    for(size_t i=0; i<request->headerCount; i++){
        free(request->headers[i].name);
        free(request->headers[i].value);
    }

    free(request->headers);
    free(request->body);
    httpRequestInit(request);
}
