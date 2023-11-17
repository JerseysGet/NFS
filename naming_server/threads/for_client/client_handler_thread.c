#include "client_handler_thread.h"

#include "../../../common/networking/requests.h"

bool isCleaning(ConnectedClient client) {
    pthread_mutex_lock(&client->cleanupLock);
    bool ret = client->isCleaningup;
    pthread_mutex_unlock(&client->cleanupLock);
    return ret;
}

void cleanupClient(ConnectedClient client) {
    lprintf("Client Thread (Alive port = %d) : Cleaning up", client->clientInitRequest.clientAlivePort);
    // close(client->clientSockfd);
    pthread_mutex_destroy(&client->cleanupLock);
}

void* clientThreadRoutine(void* arg) {
    ConnectedClient client = (ConnectedClient)arg;
    while (!isCleaningUp() && !isCleaning(client)) {
        RequestType reqType;
        RequestTypeAck ack;

        lprintf("Client Thread (Alive port = %d) : Waiting for client", client->clientInitRequest.clientAlivePort);

        if (recieveRequestType(&reqType, client->clientSockfd)) {
            killClientThread(client);
            break;
        }

        lprintf("Client Thread (Alive port = %d) : Recieved %s", client->clientInitRequest.clientAlivePort, REQ_TYPE_TO_STRING[reqType]);

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

        if (isPrivileged(reqType)) {

        } else {
            ReadRequest* castRequest = (ReadRequest*) request;
            SSInfo ssinfo;
            lockTrie();
            // assuming request type == read
            ErrorCode ret = search(castRequest->path, &ssinfo);
            unlockTrie();
            sendSSInfo(&ssinfo, client->clientSockfd);
        }

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