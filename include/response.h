#ifndef RESPONSE_H
#define RESPONSE_H

#include "client.h"

#include <stddef.h>

typedef struct{
    int statusCode;
    const char *statusText;

    const char *contentType;
    const char *body;
    size_t bodyLength;
}HttpResponse;

void httpResponseInit(HttpResponse *response);

int httpResponseSend(Client *client, HttpResponse *response);

#endif