#include "client_requests.h"


ErrorCode WriteResponseHandler(int sockfd){
    FeedbackAck fdAck;
    if(recieveFeedbackAck(&fdAck,client.nmSockfd)) {
        eprintf("Could not receive Write feedbackAck");
        return FAILURE;
    }
    lprintf("Main : FeedbackAck Received for Write : %d",fdAck);
    return SUCCESS;
}
ErrorCode ReadResponseHandler(int sockfd) {
    // char buffer[1024];
    // if(socketRecieve())
}
ErrorCode MetaDataResponseHandler(int sockfd) {
    struct stat st;
    FeedbackAck fdAck;
    if(socketRecieve(sockfd,&st,sizeof(struct stat))){
        eprintf("Could not receive metadata");
        return FAILURE;
    }
    lprintf("Main : MetaData Received");
    // print metadata
    return SUCCESS;
}
ErrorCode ListResponseHandler(int sockfd) {
    
}

ErrorCode inputAndSendRequest() {
    ErrorCode ret = SUCCESS;
    RequestType type;
    void* request = NULL;
    while (request == NULL) request = inputRequest(&type);
    lprintf("Main : sending RequestType..");
    if ((ret = sendRequestType(&type, client.nmSockfd))) {
        eprintf("Could not send request type\n");
        goto destroy_request;
    }

    bool recievedAck;
    RequestTypeAck requestTypeAck;
    recieveRequestTypeAck(&requestTypeAck, client.nmSockfd, TIMEOUT_MILLIS, &recievedAck);

    if (!recievedAck) {
        eprintf("RequestTypeAck timed out\n");
        ret = FAILURE;
        goto destroy_request;
    }
    lprintf("Main : received RequestType ack");
    if ((ret = sendRequest(type, request, client.nmSockfd))) {
        eprintf("Could not send request\n");
        goto destroy_request;
    }
    lprintf("Main : request sent");
    if(isPrivileged(type)){
        FeedbackAck fdAck;
        if(recieveFeedbackAck(&fdAck,client.nmSockfd)) {
            eprintf("Could not Receive feedbackAck from SS");
            ret = FAILURE;
            goto destroy_request;
        }
    }
    else {
        SSInfo ssinfo;
        if (recieveSSInfo(&ssinfo, client.nmSockfd)) {
            eprintf("Could not recieve ssinfo\n");
            ret = FAILURE;
            goto destroy_request;
        }
        lprintf("Main : recieved ssinfo ssClientPort = %d, ssPassivePort = %d", ssinfo.ssClientPort, ssinfo.ssPassivePort);
        int sockfd;
        if(createActiveSocket(&sockfd)){
            ret = FAILURE;
            goto destroy_request;
        }
        lprintf("Main : Active Socket Created");
        if(connectToServer(sockfd, ssinfo.ssClientPort)){
            ret = FAILURE;
            goto destroy_request;
        }
        lprintf("Main : Connected to SS");
        if((ret = sendRequestType(&type,sockfd))){
            goto destroy_request;
        }
        lprintf("Main : RequestType sent to SS");
        bool RecAck;
        RequestTypeAck typeAck;
        if(recieveRequestTypeAck(&typeAck,sockfd,TIMEOUT_MILLIS,&RecAck));

        if(!RecAck){
            eprintf("RequestTypeAck timed out\n");
            ret = FAILURE;
            goto destroy_request;
        }
        
        if((ret = sendRequest(type,request,sockfd))){
            eprintf("Could not send Request to SS");
            goto destroy_request;
        }
        
        switch(type) {
            case REQUEST_WRITE:
                if((ret = WriteResponseHandler(sockfd))){
                    ret = FAILURE;
                    goto destroy_request;
                }
            case REQUEST_READ:
                if((ret = ReadResponseHandler(sockfd))){
                    ret = FAILURE;
                    goto destroy_request;
                }
            case REQUEST_METADATA:
                if((ret = MetaDataResponseHandler(sockfd))){
                    ret = FAILURE;
                    goto destroy_request;
                }    
            case REQUEST_LIST:
                if((ret = ListResponseHandler(sockfd))){
                    ret = FAILURE;
                    goto destroy_request;
                }
            default:
        }

    }

destroy_request:
    destroyRequest(request);
    return ret;
}

