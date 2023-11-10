#include "storage_server.h"

#include <stdio.h>

ErrorCode initSS(StorageServer* ss) {
    /*
        TODO
        1. initialize ss->aliveSockfd by creating passive socket
        2. get ss->aliveSockPort with getPort()
        3. initialize ss->clientSockfd by creating passive socket
        4. get ss->aliveSockPort with getPort()
        5. Call inputPaths()
        6. initialize ss->nmSockfd by creating active socket
    */
    return SUCCESS;
}

void destroySS(StorageServer* ss) {
    /*
        TODO
        close all sockfds
    */
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