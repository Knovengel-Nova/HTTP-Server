#include "strings.h"

#include <stddef.h>
#include <ctype.h>

const char *findcrlf(const char *start, const char *end){
    while(start+1 < end){
        if(start[0] == '\r' && start[1] == '\n'){
            return start;
        }

        start++;
    }
    return NULL;
}

int stringEqualsIgnoreCase(const char *a, const char *b){
    if(a == NULL || b == NULL){
        return 0;
    }

    while(*a != '\0' && *b != '\0'){
        if(tolower((unsigned char)*a) != tolower((unsigned char)*b)){
            return 0;
        }

        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

const char *findHeaderEnd(const char *data, size_t length){
    if(data == NULL || length<4){
        return NULL;
    }

    for(size_t i=0; i<=length-4; i++){
        if(data[i] == '\r' && data[i+1] == '\n' && data[i+2] == '\r' && data[i+3] == '\n'){
            return data +i;
        }
    }

    return NULL;
}

void trimWhitespace(const char **start, const char **end){
    while(*start < *end && (**start == ' ' || **start == '\t')){
        (*start)++;
    }

    while(*end > *start && ((*(*end - 1) == ' ') || (*(*end -1) == '\t'))){
        (*end)--;
    }
}