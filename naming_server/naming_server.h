#ifndef __NAMING_SERVER_H
#define __NAMING_SERVER_H

#include <stdlib.h>

#include "../common/error/error.h"
#include "../common/networking/nm_ss/ss_connect.h"
#include "../common/print/logging.h"
#include "./threads/ss_listener_thread.h"

extern FILE* logFile;

#define FATAL_EXIT   \
    do {             \
        destroyNM(); \
        exit(1);     \
    } while (0)

#define UNUSED(x) \
    do {          \
        (void)x;  \
    } while (0);

#define MAX_STORAGE_SERVERS 100
typedef struct ConnectedSS {
    int count;
    SSInitRequest storageServers[MAX_STORAGE_SERVERS];
    int storageServerSockfds[MAX_STORAGE_SERVERS];
} ConnectedSS;

typedef struct NamingServer {
    int ssListenerSockfd;            /* Socketfd for ss listener passive port */
    ConnectedSS connectedSS;         /* Stores connected storage servers, must be locked for synchronization */
    pthread_mutex_t connectedSSLock; /* Lock for connectedSS */
    pthread_t ssListener;            /* Storage server listener thread */
} NamingServer;

/* Unique naming server instance */
extern NamingServer namingServer;

ErrorCode initNM();
void destroyNM();

#endif