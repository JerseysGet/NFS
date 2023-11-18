#include "storage_server.h"

#include <stdio.h>

#include "../common/networking/networking.h"
#include "../common/print/logging.h"

#define JOIN_IF_CREATED(T, RET)      \
    do {                             \
        if (T) pthread_join(T, RET); \
    } while (0)

StorageServer ss;

ErrorCode initSS() {
    ss.isCleaningup = 0;
    ss.exitCode = 0;

    if (initLogger("logs/storage_server/", false)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }

    if (startLogging()) {
        eprintf("Could not start logging\n");
        return FAILURE;
    }

    if (initAliveSocketThread(&ss.aliveThread)) {
        return FAILURE;
    }


    lprintf("Main : Creating Passive Socket for SS's Client Socket");
    if (createPassiveSocket(&ss.clientSockfd, 0)) {
        return FAILURE;
    }

    if (getPort(ss.clientSockfd, &ss.clientSockPort)) {
        return FAILURE;
    }

    lprintf("Main : Creating Passive Socket for SS's Passive Socket");
    if (createPassiveSocket(&ss.passiveSockfd, 0)) {
        return FAILURE;
    } else {
        lprintf("Main : Getting port for SS's Passive Socket");
        if (getPort(ss.passiveSockfd, &ss.passiveSockPort))
            return FAILURE;
    }
    inputPaths(ss);
    lprintf("Main : Creating Active Socket for SS's NM Socket");
    if (createActiveSocket(&ss.nmSockfd))
        return FAILURE;

    return SUCCESS;



destroy_alive_socket_thread:

destroy_logging:
    endLogging();

destroy_logger:
    destroyLogger();

    return FAILURE;
}

void destroySS() {
    lprintf("Main : Closing all sockfds of SS");
    endLogging();
    destroyLogger();
    closeSocket(ss->aliveSockfd);
    closeSocket(ss->nmSockfd);
    closeSocket(ss.clientSockfd);
    closeSocket(ss.nmSockfd);
    exit(ss.exitCode);
}

ErrorCode inputPaths() {
    printf("Enter the number of paths: ");
    scanf("%d", &ss.paths.count);
    for (int i = 0; i < ss.paths.count; i++) {
        printf("Enter path %d: ", i + 1);
        scanf("%s", ss.paths.pathList[i]);
    }
    return SUCCESS;
}

ErrorCode connectToNM() {
    SSInitRequest req;
    req.paths = ss->paths;
    req.SSAlivePort = ss->aliveSockPort;
    req.SSPassivePort = ss->passiveSockPort;
    req.SSClientPort = ss->cltThread.clientSockPort;
    connectToServer(ss->nmSockfd, SS_LISTEN_PORT);
    if (sendSSRequest(ss->nmSockfd, &req)) {
    req.paths = ss.paths;
    req.SSClientPort = ss.clientSockPort;
    req.SSPassivePort = ss.passiveSockPort;
    connectToServer(ss.nmSockfd, SS_LISTEN_PORT);
    if (sendSSRequest(ss.nmSockfd, &req)) {
        return FAILURE;
    }

    return SUCCESS;
}

void initiateCleanup(ErrorCode exitCode) {
    ss.exitCode = exitCode;
    ss.isCleaningup = 1;
void initiateCleanup(ErrorCode exitCode){

}

bool isCleaningUp() {
    return false;
}

void signalSuccess() {
    ss.isCleaningup = 1;
    destroySS();
}