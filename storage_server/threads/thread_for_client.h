#ifndef __THREAD_FOR_CLIENT_H
#define __THREAD_FOR_CLIENT_H

#include <pthread.h>

typedef struct ThreadForClient {
    pthread_t thread;
    int clientSockfd;   /* Passive socket for client to connect to SS */
    int clientSockPort; /* Port for clientSockfd */
} ThreadForClient;

void initThreadForClient(ThreadForClient* thread);
void startThreadForClient(ThreadForClient* thread);

#endif