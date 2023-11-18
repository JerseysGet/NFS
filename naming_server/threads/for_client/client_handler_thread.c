#include "client_handler_thread.h"

#include "../../../common/networking/requests.h"
#include <sys/socket.h>

bool isCleaning(ConnectedClient client) {
    pthread_mutex_lock(&client->cleanupLock);
    bool ret = client->isCleaningup;
    pthread_mutex_unlock(&client->cleanupLock);
    return ret;
}

void cleanupClient(ConnectedClient client) {
    lprintf("Client Thread (Alive port = %d) : Cleaning up", client->clientInitRequest.clientAlivePort);
    pthread_mutex_destroy(&client->cleanupLock);
}

void* clientThreadRoutine(void* arg) {
    ConnectedClient client = (ConnectedClient)arg;
    while (!isCleaningUp() && !isCleaning(client)) {
        RequestType reqType;
        RequestTypeAck ack;

        lprintf("Client Thread (Alive port = %d) : Waiting for client", client->clientInitRequest.clientAlivePort);
        if (isCleaning(client)) break;

        if (recieveRequestType(&reqType, client->clientSockfd)) {
            killClientThread(client);
            break;
        }
        lprintf("Client Thread (Alive port = %d) : Recieved %d", client->clientInitRequest.clientAlivePort, reqType);
        lprintf("Client Thread (Alive port = %d) : Recieved %d %s", client->clientInitRequest.clientAlivePort, reqType, REQ_TYPE_TO_STRING[reqType]);

        if (sendRequestTypeAck(&ack, client->clientSockfd)) {
            killClientThread(client);
            break;
        }

        lprintf("Client Thread (Alive port = %d) : Sent RequestTypeAck", client->clientInitRequest.clientAlivePort);

        void* request = allocateRequest(reqType);
        if (recieveRequest(reqType, request, client->clientSockfd)) {
            free(request);
            killClientThread(client);
            break;
        }
        lprintf("Client Thread (Alive port = %d) : Recieved Request", client->clientInitRequest.clientAlivePort);
        
        /* LIST REQUEST HANDLED DIFFERENT FROM REST */
        if (reqType == REQUEST_LIST) {
            ListRequest* castRequest = (ListRequest*) request;
            ListResponse response;
            response.list_cnt = 0;
            lockTrie();
            char** ret = getChildren(castRequest->path, &response.list_cnt);
            unlockTrie();
            for (int i = 0; i < response.list_cnt; i++) {
                strcpy(response.list[i], ret[i]);
            }
            if (ret) {
                for (int i = 0; i < response.list_cnt; i++) free(ret[i]); 
                free(ret);
            }
            socketSend(client->clientSockfd, &response, sizeof(ListResponse));
            FeedbackAck ack;
            ack.errorCode = 0;
            if (ret == NULL) ack.errorCode = EPATHNOTFOUND;
            sendFeedbackAck(&ack, client->clientSockfd);
            goto free_request;
        }

        if (isPrivileged(reqType)) {
            // assuming request type == create
            CreateRequest* castRequest = (CreateRequest*)request;
            SSInfo ssinfo;
            lockTrie();
            ErrorCode ret = search(castRequest->path, &ssinfo);
            unlockTrie();
            if (ret == EPATHNOTFOUND) {
                FeedbackAck ack;
                ack.errorCode = ret;
                sendFeedbackAck(&ack, client->clientSockfd);
            } else {
                int sockfd;
                createActiveSocket(&sockfd);
                connectToServer(sockfd, ssinfo.ssPassivePort);
                sendRequestType(&reqType, sockfd);
                sendRequest(reqType, request, sockfd);
                FeedbackAck ack;
                recieveFeedbackAck(&ack, sockfd);
                sendFeedbackAck(&ack, client->clientSockfd);
                close(sockfd);
            }
        } else {
            ReadRequest* castRequest = (ReadRequest*)request;
            SSInfo ssinfo;
            lockTrie();
            // assuming request type == read
            ErrorCode ret = search(castRequest->path, &ssinfo);
            unlockTrie();
            lprintf("Sending ssinfo with ssclientInfo = %d, sspassiveport = %d", ssinfo.ssClientPort, ssinfo.ssPassivePort);
            sendSSInfo(&ssinfo, client->clientSockfd);
        }
        free_request:
        free(request);
    }
    cleanupClient(client);
    return NULL;
}

void startClientThread(ConnectedClient client) {
    pthread_create(&client->thread, NULL, clientThreadRoutine, (void*)client);
}

void killClientThread(ConnectedClient client) {
    pthread_mutex_lock(&client->cleanupLock);
    client->isCleaningup = true;
    pthread_mutex_unlock(&client->cleanupLock);
}