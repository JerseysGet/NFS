#include "naming_server.h"

#include "../common/networking/networking.h"

NamingServer namingServer;
FILE* logFile;

ErrorCode initNM() {
    createLogFile(logFile, NM_LOGS);
    namingServer.connectedSS.count = 0;
    int ret;
    if ((ret = pthread_mutex_init(&namingServer.connectedSSLock, NULL))) {
        eprintf("Could not initialize connected storage server mutex, errno = %d, %s\n", ret, strerror(ret));
        return FAILURE;
    }
    if (createPassiveSocket(&namingServer.ssListenerSockfd, SS_LISTEN_PORT)) return FAILURE;
    return SUCCESS;
}

void destroyNM() {
    fclose(logFile);
    pthread_mutex_destroy(&namingServer.connectedSSLock);
    close(namingServer.ssListenerSockfd);
}