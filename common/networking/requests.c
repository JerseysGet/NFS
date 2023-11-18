#include "requests.h"

ErrorCode sendRequestType(RequestType* requestType, int sockfd) {
    if (socketSend(sockfd, requestType, sizeof(RequestType))) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode recieveRequestType(RequestType* requestType, int sockfd) {
    if (socketRecieve(sockfd, requestType, sizeof(RequestType))) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode sendRequestTypeAck(RequestTypeAck* ack, int sockfd) {
    if (socketSend(sockfd, ack, sizeof(RequestTypeAck))) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode recieveRequestTypeAck(RequestTypeAck* ack, int sockfd, int millis, bool* recieved) {
    if (socketRecieveTimeout(sockfd, ack, sizeof(RequestTypeAck), millis, recieved)) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode __sendRequest(void* request, size_t requestSize, int sockfd) {
    if (socketSend(sockfd, request, requestSize)) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode __recieveRequest(void* request, size_t requestSize, int sockfd) {
    if (socketRecieve(sockfd, request, requestSize)) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode recieveRequest(RequestType requestType, void* request, int sockfd) {
    switch (requestType) {
        case REQUEST_COPY:
            return recieveCopyRequest(request, sockfd);
        case REQUEST_CREATE:
            return recieveCreateRequest(request, sockfd);
        case REQUEST_DELETE:
            return recieveDeleteRequest(request, sockfd);
        // case REQUEST_PERMISSION:
        //     return recievePermRequest(request, sockfd);
        case REQUEST_READ:
            return recieveReadRequest(request, sockfd);
        // case REQUEST_SIZE:
        //     return recieveSizeRequest(request, sockfd);
        case REQUEST_WRITE:
            return recieveWriteRequest(request, sockfd);
        case REQUEST_METADATA:
            return recieveMDRequest(request, sockfd);
    }

    return FAILURE;
}

void* allocateRequest(RequestType requestType) {
    switch (requestType) {
        case REQUEST_COPY:
            return calloc(1, sizeof(CopyRequest));
        case REQUEST_CREATE:
            return calloc(1, sizeof(CreateRequest));
        case REQUEST_DELETE:
            return calloc(1, sizeof(DeleteRequest));
        // case REQUEST_PERMISSION:
        //     return calloc(1, sizeof(PermRequest));
        case REQUEST_READ:
            return calloc(1, sizeof(ReadRequest));
        // case REQUEST_SIZE:
        //     return calloc(1, sizeof(SizeRequest));
        case REQUEST_WRITE:
            return calloc(1, sizeof(WriteRequest));
        case REQUEST_METADATA:
            return calloc(1, sizeof(MDRequest));
    }

    return NULL;
}

ErrorCode sendRequest(RequestType requestType, void* request, int sockfd) {
    switch (requestType) {
        case REQUEST_COPY:
            return sendCopyRequest(request, sockfd);
        case REQUEST_CREATE:
            return sendCreateRequest(request, sockfd);
        case REQUEST_DELETE:
            return sendDeleteRequest(request, sockfd);
        // case REQUEST_PERMISSION:
        //     return sendPermRequest(request, sockfd);
        case REQUEST_READ:
            return sendReadRequest(request, sockfd);
        // case REQUEST_SIZE:
        //     return sendSizeRequest(request, sockfd);
        case REQUEST_WRITE:
            return sendWriteRequest(request, sockfd);
        case REQUEST_METADATA:
            return sendMDRequest(request, sockfd);
    }

    return FAILURE;
}

ErrorCode sendFeedbackAck(FeedbackAck* ack, int sockfd) {
    if (socketSend(sockfd, ack, sizeof(FeedbackAck))) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode recieveFeedbackAck(FeedbackAck* ack, int sockfd) {
    if (socketRecieve(sockfd, ack, sizeof(FeedbackAck))) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode sendSSInfo(SSInfo* ssinfo, int sockfd) {
    if (socketSend(sockfd, ssinfo, sizeof(SSInfo))) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode recieveSSInfo(SSInfo* ssinfo, int sockfd) {
    if (socketRecieve(sockfd, ssinfo, sizeof(SSInfo))) {
        return FAILURE;
    }
    return SUCCESS;
}
