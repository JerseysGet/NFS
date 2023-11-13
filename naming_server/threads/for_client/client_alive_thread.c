#include "client_alive_thread.h"

#include "../../../common/networking/networking.h"

/* Termicnates naming server in case of fatal errors */
void* clientAliveRoutine(void* arg) {
    UNUSED(arg);
    ConnectedClients* connectedClients = &namingServer.connectedClients;
    int toRemoveIndices[MAX_CLIENTS];

    pthread_mutex_lock(&namingServer.cleanupLock);
    while (!namingServer.isCleaningup) {
        pthread_mutex_unlock(&namingServer.cleanupLock);

        int toRemoveCount = 0;
        // printf("Client_Alive trying to lock connectedClients\n");
        pthread_mutex_lock(&namingServer.connectedClientsLock);
        for (int i = 0; i < connectedClients->count; i++) {
            int clientAlivePort = connectedClients->clients[i].clientAlivePort;

            int tempSockfd;
            if (createActiveSocket(&tempSockfd)) {
                eprintf("Could not create active socket to test if client with alive port %d is alive\n", clientAlivePort);
                FATAL_EXIT;
            }

            if (!canConnectToServer(tempSockfd, clientAlivePort)) {
                lprintf("Client_Alive: Client with alive port %d disconnected", clientAlivePort);
                toRemoveIndices[toRemoveCount++] = i;
            }

            if (closeSocket(tempSockfd)) {
                eprintf("Could not close socket\n");
                FATAL_EXIT;
            }
        }

        int newClientCount = 0;
        int ptr = 0;
        for (int i = 0; i < connectedClients->count; i++) {
            if (ptr < toRemoveCount && i == toRemoveIndices[ptr]) {  // ignore i
                ptr++;
            } else {
                connectedClients->clients[newClientCount] = connectedClients->clients[i];
                connectedClients->clientSockfds[newClientCount] = connectedClients->clientSockfds[i];
                newClientCount++;
            }
        }
        connectedClients->count = newClientCount;
        pthread_mutex_unlock(&namingServer.connectedClientsLock);

        // printf("Client_Alive trying to lock cleanup\n");
        pthread_mutex_lock(&namingServer.cleanupLock);
    }
    pthread_mutex_unlock(&namingServer.cleanupLock);

    lprintf("Client_Alive : Cleaning up");
    return NULL;
}

ErrorCode createClientAliveThread(pthread_t* aliveThread) {
    int ret;
    if ((ret = pthread_create(aliveThread, NULL, clientAliveRoutine, NULL))) {
        eprintf("Could not create client alive thread, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}