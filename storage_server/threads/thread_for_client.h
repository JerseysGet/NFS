#ifndef __THREAD_FOR_CLIENT_H
#define __THREAD_FOR_CLIENT_H

#include <pthread.h>

#include "../../common/networking/requests.h"
#include "../../common/networking/ack/feedback_ack.h"
#include "../../common/networking/ack/request_type_ack.h"
#include "../operations/operations.h"

typedef struct ThreadForClient {
    pthread_t thread;
    int clientSockfd;   /* Passive socket for client to connect to SS */
    int clientSockPort; /* Port for clientSockfd */
} ThreadForClient;

void initThreadForClient(ThreadForClient* thread);
void startThreadForClient(ThreadForClient* thread);

#endif