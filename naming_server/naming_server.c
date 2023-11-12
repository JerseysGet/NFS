#include "naming_server.h"

#include <assert.h>

#include "../common/networking/networking.h"

NamingServer namingServer;

ErrorCode initConnectedSS() {
    namingServer.connectedSS.count = 0;
    int ret;
    if ((ret = pthread_mutex_init(&namingServer.connectedSSLock, NULL))) {
        eprintf("Could not initialize connected storage server mutex, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}

ErrorCode initConnectedClients() {
    namingServer.connectedClients.count = 0;
    int ret;
    if ((ret = pthread_mutex_init(&namingServer.connectedClientsLock, NULL))) {
        eprintf("Could not initialize connected client mutex, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    return SUCCESS;
}

ErrorCode initNM() {
    if (initLogger("logs/naming_server/", false)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }

    if (startLogging()) {
        eprintf("Could not start logging\n");
        return FAILURE;
    }

    if (initConnectedSS()) return FAILURE;
    lprintf("Main : Creating passive socket (ss listener) on port = %d", SS_LISTEN_PORT);
    if (createPassiveSocket(&namingServer.ssListenerSockfd, SS_LISTEN_PORT)) return FAILURE;

    if (initConnectedClients()) return FAILURE;
    lprintf("Main : Creating passive socket (client listener) on port = %d", CLIENT_LISTEN_PORT);
    if (createPassiveSocket(&namingServer.clientListenerSockfd, CLIENT_LISTEN_PORT)) return FAILURE;

    return SUCCESS;
}

void destroyNM() {
    destroyLogger();
    pthread_mutex_destroy(&namingServer.connectedSSLock);
    pthread_mutex_destroy(&namingServer.connectedClientsLock);
    close(namingServer.ssListenerSockfd);
}