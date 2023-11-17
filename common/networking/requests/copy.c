#include "copy.h"

ErrorCode sendCopyRequest(CopyRequest* request, int sockfd) {
    return __sendRequest(request, sizeof(CopyRequest), sockfd);
}

ErrorCode recieveCopyRequest(CopyRequest* request, int sockfd) {
    return __recieveRequest(request, sizeof(CopyRequest), sockfd);
}