#include "client_listener_thread.h"

#include "../../../common/networking/networking.h"

/* Terminates naming server in case of fatal errors */
void* clientListenerRoutine(void* arg) {
    UNUSED(arg);
    ConnectedClients* connectedClients = &namingServer.connectedClients;
    while (1) {
        LOG("Waiting for client...\n");
        int clientSockfd;
        if (acceptClient(namingServer.clientListenerSockfd, &clientSockfd)) FATAL_EXIT;
        LOG("Client connected\n");
        ClientInitRequest recievedReq;
        if (recieveClientRequest(clientSockfd, &recievedReq)) FATAL_EXIT;
        LOG("ClientRequest recieved:\n");

        pthread_mutex_lock(&namingServer.connectedClientsLock);
        connectedClients->clients[connectedClients->count] = recievedReq;
        LOG("Passive port = %d, Alive port = %d\n", recievedReq.clientPassivePort, recievedReq.clientAlivePort);
        connectedClients->clientSockfds[connectedClients->count] = clientSockfd;
        connectedClients->count++;
        if (connectedClients->count > MAX_CLIENTS) {
            eprintf("Too many clients\n");
            FATAL_EXIT;
        }
        pthread_mutex_unlock(&namingServer.connectedClientsLock);
    }
}

ErrorCode createClientListenerThread(pthread_t* listenerThread) {
    int ret;
    if ((ret = pthread_create(listenerThread, NULL, clientListenerRoutine, NULL))) {
        eprintf("Could not create client listener thread, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}