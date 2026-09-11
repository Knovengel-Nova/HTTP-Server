#ifndef REQUEST_H
#define REQUEST_H

#include <stddef.h>

typedef struct{
    char *name;
    char *value;
}HttpHeader;

typedef struct{
    char *method;
    char *target;
    char *version;

    HttpHeader *headers;
    size_t headerCount;

    char *body;
    size_t bodyLength;
}HttpRequest;

void httpRequestInit(HttpRequest *request);

void httpRequestDestroy(HttpRequest *request);

const char *httpRequestGetHeader(const HttpRequest *request, const char *name);

int httpRequestParse(const char *data, size_t length, HttpRequest *request);

#endif