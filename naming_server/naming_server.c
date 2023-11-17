#include "naming_server.h"

#include <assert.h>
#include <sys/socket.h>

#include "../common/networking/networking.h"

NamingServer namingServer;

#define JOIN_IF_CREATED(T, RET)      \
    do {                             \
        if (T) pthread_join(T, RET); \
    } while (0)

ErrorCode initConnectedSS() {
    namingServer.connectedSS.count = 0;
    int ret;
    if ((ret = pthread_mutex_init(&namingServer.connectedSSLock, NULL))) {
        eprintf("Could not initialize connected storage server mutex, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}

void destroyConnectedSS() {
    pthread_mutex_destroy(&namingServer.connectedSSLock);
}

void signalSuccess();

ErrorCode initNM() {
    namingServer.ssListener = 0;
    namingServer.ssAliveChecker = 0;
    namingServer.clientListener = 0;
    namingServer.clientAliveChecker = 0;
    namingServer.isCleaningup = false;

    pthread_mutex_init(&namingServer.cleanupLock, NULL);
    if (initLogger("logs/naming_server/", false)) {
        eprintf("Could not create log file\n");
        exit(FAILURE);
    }

    if (startLogging()) {
        eprintf("Could not start logging\n");
        goto destroy_logger;
    }

    lprintf("Main : Initializing trie");
    initTrie();

    initEscapeHatch(signalSuccess);

    if (initConnectedSS()) {
        goto destroy_escape_hatch;
    }

    lprintf("Main : Creating passive socket (ss listener) on port = %d", SS_LISTEN_PORT);
    if (createPassiveSocket(&namingServer.ssListenerSockfd, SS_LISTEN_PORT)) {
        goto destroy_connected_SS;
    }

    if (initConnectedClients(&namingServer.connectedClients)) {
        goto destroy_SS_listener;
    }

    lprintf("Main : Creating passive socket (client listener) on port = %d", CLIENT_LISTEN_PORT);
    if (createPassiveSocket(&namingServer.clientListenerSockfd, CLIENT_LISTEN_PORT)) {
        goto destroy_connected_clients;
    }
    return SUCCESS;

    // destroy_client_listener:
    //     close(namingServer.clientListenerSockfd);

destroy_connected_clients:
    destroyConnectedClients(&namingServer.connectedClients);

destroy_SS_listener:
    close(namingServer.ssListenerSockfd);

destroy_connected_SS:
    destroyConnectedSS();

destroy_escape_hatch:
    destroyEscapeHatch();
    endLogging();

destroy_logger:
    destroyLogger();
    pthread_mutex_destroy(&namingServer.cleanupLock);

    exit(FAILURE);
}

void destroyNM() {
    JOIN_IF_CREATED(namingServer.clientAliveChecker, NULL);
    JOIN_IF_CREATED(namingServer.ssAliveChecker, NULL);

    shutdown(namingServer.ssListenerSockfd, SHUT_RDWR);
    shutdown(namingServer.clientListenerSockfd, SHUT_RDWR);

    JOIN_IF_CREATED(namingServer.clientListener, NULL);
    JOIN_IF_CREATED(namingServer.ssListener, NULL);

    destroyConnectedClients(&namingServer.connectedClients);

    lprintf("Main : Cleaning up trie");
    destroyTrie();

    lprintf("Main : Exit request to logger thread");
    endLogging();
    destroyLogger();
    JOIN_IF_CREATED(getLoggingThread(), NULL);
    

    pthread_mutex_destroy(&namingServer.connectedSSLock);
    pthread_mutex_destroy(&namingServer.cleanupLock);

    destroyEscapeHatch();
    exit(namingServer.exitCode);
}

bool isCleaningUp() {
    pthread_mutex_lock(&namingServer.cleanupLock);
    bool ret = namingServer.isCleaningup;
    pthread_mutex_unlock(&namingServer.cleanupLock);
    return ret;
}

void initiateCleanup(ErrorCode exitCode) {
    pthread_mutex_lock(&namingServer.cleanupLock);
    namingServer.exitCode = exitCode;
    namingServer.isCleaningup = true;
    pthread_mutex_unlock(&namingServer.cleanupLock);
}

void signalSuccess() {
    initiateCleanup(SUCCESS);
    destroyNM();
}