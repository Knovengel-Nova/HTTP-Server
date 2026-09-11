// #define _POSIX_C_SOURCE 200809L

#include "request.h"
#include "strings.h"

#include <stdlib.h>
#include <string.h>

const char *httpRequestGetHeader(const HttpRequest *request, const char *name){
    if(request == NULL|| name == NULL){
        return NULL;
    }

    for(size_t i=0; i<request->headerCount; i++){
        if(stringEqualsIgnoreCase(request->headers[i].name, name)){
            return request->headers[i].value;
        }
    }

    return NULL;
}

static int parseRequestLine(const char *start, const char *end, HttpRequest *request){
    const char *space1 = memchr(start, ' ', end - start);
    if(space1 == NULL){
        return -1;
    }

    const char *space2 = memchr(space1 + 1, ' ', end - (space1 + 1));
    if(space2 == NULL){
        return -1;
    }

    request->method  = strndup(start, space1 - start);
    request->target  = strndup(space1 + 1, space2 - space1 - 1);
    request->version = strndup(space2 + 1, end - space2 - 1);

    if(request->method == NULL || request->target == NULL || request->version == NULL){
        return -1;
    }

    return 0;
}

static int addHeader(HttpRequest *request, const char *nameStart, size_t nameLength, const char *valueStart, size_t valueLength){
    HttpHeader *headers = realloc(request->headers, (request->headerCount + 1) * sizeof(HttpHeader));
    if(headers == NULL){
        return -1;
    }
    request->headers = headers;

    HttpHeader *header = &request->headers[request->headerCount];
    header->name  = strndup(nameStart, nameLength);
    header->value = strndup(valueStart, valueLength);

    if(header->name == NULL || header->value == NULL){
        free(header->name);
        free(header->value);
        return -1;
    }

    request->headerCount++;
    return 0;
}

/* Parses headers starting at 'start'. On success, *bodyStart points just
 * past the CRLF of the terminating blank line. */
static int parseHeaders(const char *start, const char *end, HttpRequest *request, const char **bodyStart){
    const char *current = start;

    while(current < end){
        const char *lineEnd = findcrlf(current, end);
        if(lineEnd == NULL){
            return -1; 
        }

        if(lineEnd == current){
            *bodyStart = current + 2; 
            return 0;
        }

        const char *colon = memchr(current, ':', lineEnd - current);
        if(colon == NULL){
            return -1;
        }

        const char *nameStart = current;
        const char *nameEnd = colon;
        if(nameEnd == nameStart){
            return -1; 
        }

        const char *valueStart = colon + 1;
        const char *valueEnd = lineEnd;
        trimWhitespace(&valueStart, &valueEnd);

        if(addHeader(request, nameStart, (size_t)(nameEnd - nameStart), valueStart, (size_t)(valueEnd - valueStart)) != 0){
            return -1;
        }

        current = lineEnd + 2;
    }

    return -1; 
}

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
    if(request == NULL){
        return;
    }

    free(request->method);
    free(request->target);
    free(request->version);

    for(size_t i = 0; i < request->headerCount; i++){
        free(request->headers[i].name);
        free(request->headers[i].value);
    }

    free(request->headers);
    free(request->body);

    httpRequestInit(request);
}

int httpRequestParse(const char *data, size_t length, HttpRequest *request){
    if(data == NULL || request == NULL || length == 0){
        return -1;
    }

    const char *end = data + length;

    const char *requestLineEnd = findcrlf(data, end);
    if(requestLineEnd == NULL){
        return -1;
    }

    if(parseRequestLine(data, requestLineEnd, request) != 0){
        httpRequestDestroy(request);
        return -1;
    }

    const char *headerStart = requestLineEnd + 2;
    const char *bodyStart = NULL;

    if(parseHeaders(headerStart, end, request, &bodyStart) != 0){
        httpRequestDestroy(request);
        return -1;
    }

    size_t bodyLength = (size_t)(end - bodyStart);
    if(bodyLength > 0){
        request->body = malloc(bodyLength);
        if(request->body == NULL){
            httpRequestDestroy(request);
            return -1;
        }
        memcpy(request->body, bodyStart, bodyLength);
    }
    request->bodyLength = bodyLength;

    return 0;
}