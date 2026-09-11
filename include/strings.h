#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

const char *findcrlf(const char *start, const char *end);

const char *findHeaderEnd(const char *data, size_t length);

int stringEqualsIgnoreCase(const char *a, const char *b);

void trimWhitespace(const char **start, const char **end);

#endif