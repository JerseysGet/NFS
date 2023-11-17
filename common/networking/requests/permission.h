#ifndef __PERMISSION_H
#define __PERMISSION_H

#include "../../error/error.h"
#include "../requests.h"

#define MAX_PATH_SIZE 256

typedef struct PermRequest {
    char path[MAX_PATH_SIZE];
} PermRequest;

ErrorCode sendPermRequest(PermRequest* request, int sockfd);
ErrorCode recievePermRequest(PermRequest* request, int sockfd);

#endif