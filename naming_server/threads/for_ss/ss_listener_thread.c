#include "ss_listener_thread.h"

#include "../../../common/networking/networking.h"

/* Terminates naming server in case of fatal errors */
void* ssListenerRoutine(void* arg) {
    UNUSED(arg);
    ConnectedSS* connectedSS = &namingServer.connectedSS;
    while (1) {
        lprintf("SS_listener : Waiting for storage server...");
        int ssSockfd;
        if (acceptClient(namingServer.ssListenerSockfd, &ssSockfd)) FATAL_EXIT;
        lprintf("SS_listener : Storage server connected");
        SSInitRequest recievedReq;
        if (recieveSSRequest(ssSockfd, &recievedReq)) FATAL_EXIT;
        lprintf("SS_listener : Recieved Alive port = %d, Passive port = %d, Client port = %d", recievedReq.SSAlivePort, recievedReq.SSPassivePort, recievedReq.SSClientPort);
        printf("path count = %d\n", recievedReq.paths.count);
        for (int i = 0; i < recievedReq.paths.count; i++) {
            printf("\t%s\n", recievedReq.paths.pathList[i]);
        }

        pthread_mutex_lock(&namingServer.connectedSSLock);
        connectedSS->storageServers[connectedSS->count] = recievedReq;
        connectedSS->storageServerSockfds[connectedSS->count] = ssSockfd;
        connectedSS->count++;
        if (connectedSS->count > MAX_STORAGE_SERVERS) {
            eprintf("Too many storage servers\n");
            FATAL_EXIT;
        }
        pthread_mutex_unlock(&namingServer.connectedSSLock);
    }
}

ErrorCode createSSListenerThread(pthread_t* listenerThread) {
    int ret;
    if ((ret = pthread_create(listenerThread, NULL, ssListenerRoutine, NULL))) {
        eprintf("Could not create storage server listener thread, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}