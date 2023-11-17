#ifndef __READ_H
#define __READ_H

#include "../../error/error.h"
#include "../requests.h"

#define MAX_PATH_SIZE 256

typedef struct ReadRequest {
    char path[MAX_PATH_SIZE];
} ReadRequest;

ErrorCode sendReadRequest(ReadRequest* request, int sockfd);
ErrorCode recieveReadRequest(ReadRequest* request, int sockfd);

#endif