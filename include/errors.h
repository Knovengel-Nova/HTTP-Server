#ifndef ERRORS_H
#define ERRORS_H

#include "response.h"

void httpErrorResponse(HttpResponse *response, int statusCode);

#endif