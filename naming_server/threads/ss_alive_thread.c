#include "ss_alive_thread.h"

#include "../../common/networking/networking.h"

/* Terminates naming server in case of fatal errors */
void* ssAliveRoutine(void* arg) {
    UNUSED(arg);
    ConnectedSS* connectedSS = &namingServer.connectedSS;
    while (1) {
        int toRemoveIndices[MAX_STORAGE_SERVERS];
        int toRemoveCount = 0;
        pthread_mutex_lock(&namingServer.connectedSSLock);
        for (int i = 0; i < connectedSS->count; i++) {
            int ssAlivePort = connectedSS->storageServers[i].SSAlivePort;

            int tempSockfd;
            if (createActiveSocket(&tempSockfd)) {
                eprintf("Could not create active socket to test if storage server with alive port %d is alive\n", ssAlivePort);
                FATAL_EXIT;
            }

            if (!canConnectToServer(tempSockfd, ssAlivePort)) {
                LOG("Storage server with alive port %d disconnected\n", ssAlivePort);
                toRemoveIndices[toRemoveCount++] = i;
            } 

            if (closeSocket(tempSockfd)) {
                eprintf("Could not close socket\n");
                FATAL_EXIT;
            }

        }

        int newSSCount = 0;
        int ptr = 0;
        for (int i = 0; i < connectedSS->count; i++) {
            if (ptr < toRemoveCount && i == toRemoveIndices[ptr]) { // ignore i
                ptr++;
            } else {
                connectedSS->storageServers[newSSCount] = connectedSS->storageServers[i];
                connectedSS->storageServerSockfds[newSSCount] = connectedSS->storageServerSockfds[i];
                newSSCount++;
            }
        }
        connectedSS->count = newSSCount;
        pthread_mutex_unlock(&namingServer.connectedSSLock);
    }
}

ErrorCode createSSAliveThread(pthread_t* aliveThread) {
    int ret;
    if ((ret = pthread_create(aliveThread, NULL, ssAliveRoutine, NULL))) {
        eprintf("Could not create storage server alive thread, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}