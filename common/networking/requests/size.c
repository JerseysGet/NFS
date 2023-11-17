#include "size.h"

ErrorCode sendSizeRequest(SizeRequest* request, int sockfd) {
    return __sendRequest(request, sizeof(SizeRequest), sockfd);
}

ErrorCode recieveSizeRequest(SizeRequest* request, int sockfd) {
    return __recieveRequest(request, sizeof(SizeRequest), sockfd);
}