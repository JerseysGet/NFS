#ifndef __REQUEST_TYPE_H
#define __REQUEST_TYPE_H

#include <stdbool.h>

typedef enum RequestType {
    /* Privileged requests */
    REQUEST_CREATE,
    REQUEST_DELETE,
    REQUEST_COPY,

    /* Non privileged requests*/
    REQUEST_READ,
    REQUEST_WRITE,
    REQUEST_SIZE,
    REQUEST_PERMISSION,
} RequestType;

bool isPrivileged(RequestType type);

#endif