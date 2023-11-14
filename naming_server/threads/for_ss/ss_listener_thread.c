#include "ss_listener_thread.h"

#include "../../../common/networking/networking.h"

void addSS(ConnectedSS* connectedSS, SSInitRequest* req, int ssSockfd) {
    connectedSS->storageServers[connectedSS->count] = *req;
    connectedSS->storageServerSockfds[connectedSS->count] = ssSockfd;
    connectedSS->count++;
    if (connectedSS->count > MAX_STORAGE_SERVERS) {
        eprintf("Too many storage servers\n");
        FATAL_EXIT;
    }

    SSInfo ssinfo;
    initSSInfo(&ssinfo, req->SSClientPort, req->SSPassivePort);
    lockTrie();
    for (int i = 0; i < req->paths.count; i++) {
        addToTrie(req->paths.pathList[i], ssinfo);
    }
    unlockTrie();
}

/* Terminates naming server in case of fatal errors */
void* ssListenerRoutine(void* arg) {
    UNUSED(arg);
    ConnectedSS* connectedSS = &namingServer.connectedSS;
    while (!isCleaningUp()) {

        lprintf("SS_listener : Waiting for storage server...");
        int ssSockfd;
        if (acceptClient(namingServer.ssListenerSockfd, &ssSockfd)) {
            pthread_mutex_lock(&namingServer.cleanupLock);
            namingServer.isCleaningup = true;
            pthread_mutex_unlock(&namingServer.cleanupLock);
            break;
        }

        lprintf("SS_listener : Storage server connected");
        SSInitRequest recievedReq;
        if (recieveSSRequest(ssSockfd, &recievedReq)) {
            pthread_mutex_lock(&namingServer.cleanupLock);
            namingServer.isCleaningup = true;
            pthread_mutex_unlock(&namingServer.cleanupLock);
            break;
        }

        lprintf("SS_listener : Recieved Alive port = %d, Passive port = %d, Client port = %d", recievedReq.SSAlivePort, recievedReq.SSPassivePort, recievedReq.SSClientPort);
        
        printf("path count = %d\n", recievedReq.paths.count);
        for (int i = 0; i < recievedReq.paths.count; i++) {
            printf("\t%s\n", recievedReq.paths.pathList[i]);
        }

        // printf("ssListener trying to lock connectedSSLock\n");
        pthread_mutex_lock(&namingServer.connectedSSLock);
        addSS(connectedSS, &recievedReq, ssSockfd);
        pthread_mutex_unlock(&namingServer.connectedSSLock);
    }

    // cleaning up
    lprintf("SS_listener : Cleaning up");
    close(namingServer.ssListenerSockfd);
    return NULL;
}

ErrorCode createSSListenerThread(pthread_t* listenerThread) {
    int ret;
    if ((ret = pthread_create(listenerThread, NULL, ssListenerRoutine, NULL))) {
        eprintf("Could not create storage server listener thread, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}