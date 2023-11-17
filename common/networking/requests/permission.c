#include "permission.h"

ErrorCode sendPermRequest(PermRequest* request, int sockfd) {
    return __sendRequest(request, sizeof(PermRequest), sockfd);
}

ErrorCode recievePermRequest(PermRequest* request, int sockfd) {
    return __recieveRequest(request, sizeof(PermRequest), sockfd);
}