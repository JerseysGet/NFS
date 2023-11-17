#include "read.h"

ErrorCode sendReadRequest(ReadRequest* request, int sockfd) {
    return __sendRequest(request, sizeof(ReadRequest), sockfd);
}

ErrorCode recieveReadRequest(ReadRequest* request, int sockfd) {
    return __recieveRequest(request, sizeof(ReadRequest), sockfd);
}