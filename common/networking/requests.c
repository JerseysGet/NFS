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

ErrorCode recieveRequestTypeAck(RequestTypeAck* ack, int sockfd) {
    if (socketRecieve(sockfd, ack, sizeof(RequestTypeAck))) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode sendRequest(void* request, size_t requestSize, int sockfd) {
    if (socketSend(sockfd, requestSize, requestSize)) {
        return FAILURE;
    }
    return SUCCESS;
}

ErrorCode recieveRequest(void* request, size_t requestSize, int sockfd) {
    if (socketRecieve(sockfd, request, requestSize)) {
        return FAILURE;
    }
    return SUCCESS;
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
