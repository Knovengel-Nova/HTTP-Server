#ifndef ROUTER_H
#define ROUTER_H

#include "request.h"
#include "response.h"

int routerHandle(HttpRequest *request, HttpResponse *response);

#endif