#include "naming_server.h"

#include <assert.h>

#include "../common/networking/networking.h"

NamingServer namingServer;
FILE* logFile = NULL;

ErrorCode initNM() {
    if (createLogFile(&logFile, NM_LOGS)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }
    LOG("Initializing naming server\n");
    namingServer.connectedSS.count = 0;
    int ret;
    if ((ret = pthread_mutex_init(&namingServer.connectedSSLock, NULL))) {
        eprintf("Could not initialize connected storage server mutex, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }

    LOG("Creating passive socket for naming server on port = %d\n", SS_LISTEN_PORT);
    if (createPassiveSocket(&namingServer.ssListenerSockfd, SS_LISTEN_PORT)) return FAILURE;
    return SUCCESS;
}

void destroyNM() {
    fclose(logFile);
    pthread_mutex_destroy(&namingServer.connectedSSLock);
    close(namingServer.ssListenerSockfd);
}