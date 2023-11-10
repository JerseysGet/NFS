#include "storage_server.h"

#include <stdio.h>

#include "../common/print/logging.h"
#include "../networking/networking.h"

FILE* logFile;

ErrorCode initSS(StorageServer* ss) {
    if (createLogFile(&logFile, SS_LOGS)) {
        eprintf("Could not create log file\n");
        return FAILURE;
    }
    LOG("Creating Passive Socket for SS's Alive Socket\n");
    if (createPassiveSocket(&ss->aliveSockfd, 0)) {
        return FAILURE;
    } else {
        LOG("Getting port for SS's Alive Socket");
        if (getPort(ss->aliveSockfd, &ss->aliveSockPort))
            return FAILURE;
    }
    LOG("Creating Passive Socket for SS's Client Socket\n");
    if (createPassiveSocket(&ss->clientSockfd, 0)) {
        return FAILURE;
    } else {
        LOG("Getting port for SS's Client Socket");
        if (getPort(ss->clientSockfd, &ss->clientSockfd))
            return FAILURE;
    }
    LOG("Creating Passive Socket for SS's Passive Socket\n");
    if (createPassiveSocket(&ss->passiveSockfd, 0)) {
        return FAILURE;
    } else {
        LOG("Getting port for SS's Passive Socket");
        if (getPort(ss->passiveSockfd, &ss->passiveSockPort))
            return FAILURE;
    }
    inputPaths(ss);
    LOG("Creating Active Socket for SS's NM Socket\n");
    if (createActiveSocket(&ss->nmSockfd))
        return FAILURE;

    return SUCCESS;
}

void destroySS(StorageServer* ss) {
    closeSocket(ss->aliveSockfd);
    closeSocket(ss->clientSockfd);
    closeSocket(ss->nmSockfd);
    fclose(logFile);
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