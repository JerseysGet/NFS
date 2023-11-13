#include "client_listener_thread.h"

#include "../../../common/networking/networking.h"

void addClient(ConnectedClients* connectedClients, ClientInitRequest* req, int clientSockfd) {
    connectedClients->clients[connectedClients->count] = *req;
    connectedClients->clientSockfds[connectedClients->count] = clientSockfd;
    connectedClients->count++;
    if (connectedClients->count > MAX_CLIENTS) {
        eprintf("Too many clients\n");
        FATAL_EXIT;
    }
}

/* Terminates naming server in case of fatal errors */
void* clientListenerRoutine(void* arg) {
    UNUSED(arg);
    ConnectedClients* connectedClients = &namingServer.connectedClients;
    while (!isCleaningUp()) {
        lprintf("Client_listener : Waiting for client...");
        
        int clientSockfd;
        if (acceptClient(namingServer.clientListenerSockfd, &clientSockfd)) {
            pthread_mutex_lock(&namingServer.cleanupLock);
            namingServer.isCleaningup = true;
            pthread_mutex_unlock(&namingServer.cleanupLock);
            break;
        }

        lprintf("Client_listener : Client connected");
        ClientInitRequest recievedReq;
        if (recieveClientRequest(clientSockfd, &recievedReq)) {
            pthread_mutex_lock(&namingServer.cleanupLock);
            namingServer.isCleaningup = true;
            pthread_mutex_unlock(&namingServer.cleanupLock);
            break;
        }

        lprintf("Client_listener : Recieved Passive port = %d, Alive port = %d", recievedReq.clientPassivePort, recievedReq.clientAlivePort);

        // printf("Client_listener trying to lock connectedClientsLock\n");
        pthread_mutex_lock(&namingServer.connectedClientsLock);
        addClient(connectedClients, &recievedReq, clientSockfd);
        pthread_mutex_unlock(&namingServer.connectedClientsLock);
        // printf("Client_listener trying to lock cleanup\n");
    }

    lprintf("Client_listener : Cleaning up");
    close(namingServer.clientListenerSockfd);
    return NULL;
}

ErrorCode createClientListenerThread(pthread_t* listenerThread) {
    int ret;
    if ((ret = pthread_create(listenerThread, NULL, clientListenerRoutine, NULL))) {
        eprintf("Could not create client listener thread, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}