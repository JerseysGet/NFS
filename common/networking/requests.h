#ifndef __NM_CLIENT_REQUESTS_H
#define __NM_CLIENT_REQUESTS_H

#include "../error/error.h"
#include "ack/feedback_ack.h"
#include "ack/request_type_ack.h"
#include "ack/ssinfo.h"
#include "requests/request_type.h"

ErrorCode sendRequestType(RequestType* requestType, int sockfd);

ErrorCode recieveRequestType(RequestType* requestType, int sockfd);

ErrorCode sendRequestTypeAck(RequestTypeAck* ack, int sockfd);

ErrorCode recieveRequestTypeAck(RequestTypeAck* ack, int sockfd);

ErrorCode sendRequest(void* request, size_t requestSize, int sockfd);

ErrorCode recieveRequest(void* request, size_t requestSize, int sockfd);

ErrorCode sendFeedbackAck(FeedbackAck* ack, int sockfd);

ErrorCode recieveFeedbackAck(FeedbackAck* ack, int sockfd);

ErrorCode sendSSInfo(SSInfo* ssinfo, int sockfd);

ErrorCode recieveSSInfo(SSInfo* ssinfo, int sockfd);

#endif