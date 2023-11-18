#include "ss_requests.h"

bool isCleaning(Request request) {
    pthread_mutex_lock(&request->cleanupLock);
    bool ret = request->isCleaningup;
    pthread_mutex_unlock(&request->cleanupLock);
    return ret;
}

void* requestRoutine(void* arg) {
    Request requestThread = (Request)arg;
    RequestType type = requestThread->type;
    void* request = requestThread->request;
    int sockfd = requestThread->sockfd;
    if (!isCleaningup() && !isCleaning(requestThread)) {
        // execute the request

        // send the ack
    }
}

void startRequestThread(Request request) {
    if (pthread_create(&request->thread, NULL, requestRoutine, request)) return FALIURE;
    return SUCCESS;
}

void killRequestThread(Request request) {
    pthread_mutex_lock(&request->cleanupLock);
    request->isCleaningup = true;
    pthread_mutex_unlock(&request->cleanupLock);
}