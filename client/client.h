#ifndef __CLIENT_H
#define __CLIENT_H

#include <pthread.h>

#include "../common/error/error.h"
#include "../common/networking/networking.h"
#include "../common/networking/nm_client/client_connect.h"
#include "../common/networking/requests.h"
#include "../common/print/logging.h"
#include "../common/signals/cleanup_signal.h"

typedef struct Client {
    int passiveSockfd;   /* Temporary */
    int passiveSockPort; /* Temporary */

    int nmSockfd; /* To initialize & communicate with nm */

    int aliveSockfd;   /* Passive socket used by NM to check if client is alive */
    int aliveSockPort; /* Port for aliveSockfd */

    /* Cleanup stuff */
    pthread_mutex_t cleanupLock;
    bool isCleaningup;
    ErrorCode exitCode;
} Client;

extern Client client;

ErrorCode initClient();
void destroyClient();
ErrorCode connectToNM();

/* The pointer returned must be destroyed */
void* inputRequest(RequestType* requestType);
void destroyRequest(void* request);

#define TIMEOUT_MILLIS 5000

ErrorCode inputAndSendRequest();

#endif