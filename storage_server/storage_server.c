#include "storage_server.h"

#include <stdio.h>

#include "../common/print/logging.h"
#include "../common/networking/networking.h"

ErrorCode initSS(StorageServer* ss) {
    if (initLogger("logs/storage_server/", false)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }

    if (startLogging()) {
        eprintf("Could not start logging\n");
        return FAILURE;
    }

    lprintf("Main : Creating Passive Socket for SS's Alive Socket");
    if (createPassiveSocket(&ss->aliveSockfd, 0)) {
        return FAILURE;
    } else {
        lprintf("Main : Getting port for SS's Alive Socket");
        if (getPort(ss->aliveSockfd, &ss->aliveSockPort))
            return FAILURE;
    }
    lprintf("Main : Creating Passive Socket for SS's Client Socket");
    if (createPassiveSocket(&ss->clientSockfd, 0)) {
        return FAILURE;
    } else {
        lprintf("Main : Getting port for SS's Client Socket");
        if (getPort(ss->clientSockfd, &ss->clientSockPort))
            return FAILURE;
    }
    lprintf("Main : Creating Passive Socket for SS's Passive Socket");
    if (createPassiveSocket(&ss->passiveSockfd, 0)) {
        return FAILURE;
    } else {
        lprintf("Main : Getting port for SS's Passive Socket");
        if (getPort(ss->passiveSockfd, &ss->passiveSockPort))
            return FAILURE;
    }
    inputPaths(ss);
    lprintf("Main : Creating Active Socket for SS's NM Socket");
    if (createActiveSocket(&ss->nmSockfd))
        return FAILURE;

    return SUCCESS;
}

void destroySS(StorageServer* ss) {
    lprintf("Main : Closing all sockfds of SS");
    endLogging();
    destroyLogger();
    closeSocket(ss->aliveSockfd);
    closeSocket(ss->clientSockfd);
    closeSocket(ss->nmSockfd);
}

ErrorCode inputPaths(StorageServer* ss) {
    printf("Enter the number of paths: ");
    scanf("%d", &ss->paths.count);
    for (int i = 0; i < ss->paths.count; i++) {
        printf("Enter path %d: ", i + 1);
        scanf("%s", ss->paths.pathList[i]);
    }
    return SUCCESS;
}

ErrorCode connectToNM(StorageServer* ss) {
    SSInitRequest req;
    req.paths = ss->paths;
    req.SSAlivePort = ss->aliveSockPort;
    req.SSClientPort = ss->clientSockPort;
    req.SSPassivePort = ss->passiveSockPort;
    connectToServer(ss->nmSockfd, SS_LISTEN_PORT);
    if (sendSSRequest(ss->nmSockfd, &req)) {
        return FAILURE;
    }
    
    return SUCCESS;
}