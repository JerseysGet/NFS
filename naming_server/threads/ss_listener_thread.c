#include "ss_listener_thread.h"

#include "../../common/networking/networking.h"
#include "../naming_server.h"

/* Terminates in case of fatal errors */
void* ssListenerRoutine(void* arg) {
    UNUSED(arg);
    ConnectedSS* connectedSS = &namingServer.connectedSS;
    while (1) {
        int ssSockfd;
        if (acceptClient(namingServer.ssListenerSockfd, &ssSockfd)) FATAL_EXIT;
        socketRecieve(ssSockfd, &connectedSS->storageServers[connectedSS->count], sizeof(SSInitRequest));
        connectedSS->storageServerSockfds[connectedSS->count] = ssSockfd;
        connectedSS->count++;
        if (connectedSS->count > MAX_STORAGE_SERVERS) {
            eprintf("Too many storage servers\n");
            FATAL_EXIT;
        }
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